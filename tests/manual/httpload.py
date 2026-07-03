"""Shared HTTP probing/load helpers for webserv manual load tests.

Low-level: recv_headers, parse_content_length, http_get.
Concurrency: worker_loop (thread target) + start_workers/stop_and_report/run_workers,
used both for "keep hammering a path" (slow downloads, CGI hang checks) and for
"probe a path at a fixed rate while something else happens" (latency probes).
"""
import hashlib
import socket
import statistics
import threading
import time
from queue import Queue

OK_200 = (b"HTTP/1.1 200", b"HTTP/1.0 200")


def recv_headers(sock):
    buf = b""
    while b"\r\n\r\n" not in buf:
        chunk = sock.recv(4096)
        if not chunk:
            break
        buf += chunk
    if b"\r\n\r\n" not in buf:
        return b"", b""
    head, _, rest = buf.partition(b"\r\n\r\n")
    return head, rest


def parse_content_length(head):
    for line in head.split(b"\r\n"):
        if line.lower().startswith(b"content-length:"):
            return int(line.split(b":", 1)[1].strip())
    return None


def http_get(host, port, path, timeout, read_chunk=65536, pace_delay=0.0, stop_event=None,
             expected_sha256=None, ok_status_prefixes=OK_200):
    """GET path, draining the body.

    Returns (ok, elapsed_seconds, bytes_received, detail):
      ok=True   status matches ok_status_prefixes, body complete, checksum matches if requested
      ok=False  real failure; detail explains why (bad_status/incomplete/checksum_mismatch/conn_error)
      ok=None   aborted mid-transfer because stop_event fired - not a failure
    """
    start = time.perf_counter()
    hasher = hashlib.sha256() if expected_sha256 else None
    try:
        with socket.create_connection((host, port), timeout=timeout) as sock:
            req = f"GET {path} HTTP/1.1\r\nHost: {host}\r\nConnection: close\r\n\r\n"
            sock.sendall(req.encode())
            head, rest = recv_headers(sock)
            if not head:
                return False, time.perf_counter() - start, 0, "conn_error"
            status_ok = any(head.startswith(p) for p in ok_status_prefixes)
            content_length = parse_content_length(head)
            received = len(rest)
            if hasher:
                hasher.update(rest)
            sock.settimeout(timeout)
            while True:
                if stop_event is not None and stop_event.is_set():
                    return None, time.perf_counter() - start, received, "aborted"
                if content_length is not None and received >= content_length:
                    break
                chunk = sock.recv(read_chunk)
                if not chunk:
                    break
                received += len(chunk)
                if hasher:
                    hasher.update(chunk)
                if pace_delay:
                    time.sleep(pace_delay)

            if not status_ok:
                return False, time.perf_counter() - start, received, "bad_status"
            if content_length is not None and received != content_length:
                return False, time.perf_counter() - start, received, "incomplete"
            if hasher and hasher.hexdigest() != expected_sha256:
                return False, time.perf_counter() - start, received, "checksum_mismatch"
            return True, time.perf_counter() - start, received, None
    except (OSError, socket.timeout):
        return False, time.perf_counter() - start, 0, "conn_error"


def worker_loop(host, port, path, stop_event, results, timeout, interval=0.0, read_chunk=65536,
                 pace_delay=0.0, expected_sha256=None, ok_status_prefixes=OK_200, max_iterations=None):
    """Repeatedly GET path until stop_event fires (or max_iterations is reached),
    pushing (elapsed, ok, detail) into the results Queue. Aborted iterations are skipped."""
    count = 0
    while not stop_event.is_set() and (max_iterations is None or count < max_iterations):
        ok, elapsed, _, detail = http_get(host, port, path, timeout=timeout, read_chunk=read_chunk,
                                           pace_delay=pace_delay, stop_event=stop_event,
                                           expected_sha256=expected_sha256, ok_status_prefixes=ok_status_prefixes)
        if ok is not None:
            results.put((elapsed, ok, detail))
            count += 1
        if interval and stop_event.wait(interval):
            break


def start_workers(host, port, path, num_workers, timeout, interval=0.0, read_chunk=65536,
                   pace_delay=0.0, expected_sha256=None, ok_status_prefixes=OK_200, max_iterations=None):
    """Spin up num_workers worker_loop threads. Returns (stop_event, threads, results)."""
    stop_event = threading.Event()
    results = Queue()
    threads = [threading.Thread(target=worker_loop,
                                 args=(host, port, path, stop_event, results, timeout, interval, read_chunk,
                                       pace_delay, expected_sha256, ok_status_prefixes, max_iterations),
                                 daemon=True)
               for _ in range(num_workers)]
    for t in threads:
        t.start()
    return stop_event, threads, results


def stop_and_report(stop_event, threads, results, timeout, label):
    """Signal stop, join threads, aggregate the results Queue and print a report."""
    stop_event.set()
    for t in threads:
        t.join(timeout=timeout + 2)

    samples_ms, errors, detail_counts = [], 0, {}
    while not results.empty():
        elapsed, ok, detail = results.get()
        if ok:
            samples_ms.append(elapsed * 1000)
        else:
            errors += 1
            detail_counts[detail] = detail_counts.get(detail, 0) + 1
    report(label, samples_ms, errors, detail_counts)
    return samples_ms, errors, detail_counts


def run_workers(host, port, path, seconds, num_workers, timeout, label, interval=0.0, read_chunk=65536,
                 pace_delay=0.0, expected_sha256=None, ok_status_prefixes=OK_200):
    """Run num_workers against path for `seconds`, then stop/join/report. Blocks for `seconds`."""
    stop_event, threads, results = start_workers(host, port, path, num_workers, timeout, interval,
                                                   read_chunk, pace_delay, expected_sha256, ok_status_prefixes)
    time.sleep(seconds)
    return stop_and_report(stop_event, threads, results, timeout, label)


def report(label, samples_ms, errors, detail_counts=None):
    print(f"\n--- {label} ---")
    print(f"requests ok={len(samples_ms)} errors={errors}")
    if detail_counts:
        breakdown = ", ".join(f"{k}={v}" for k, v in sorted(detail_counts.items()))
        print(f"error breakdown: {breakdown}")
    if samples_ms:
        samples_ms.sort()

        def pct(p):
            idx = min(len(samples_ms) - 1, int(len(samples_ms) * p))
            return samples_ms[idx]

        print(f"latency ms: min={samples_ms[0]:.1f} avg={statistics.mean(samples_ms):.1f} "
              f"p50={pct(0.50):.1f} p95={pct(0.95):.1f} p99={pct(0.99):.1f} max={samples_ms[-1]:.1f}")
