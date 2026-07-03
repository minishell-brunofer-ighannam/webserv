#!/usr/bin/env python3
"""Load test for webserv focused on the big-file / poll regression.

Downloads a large static file through slow (throttled) clients while
concurrently probing a small path. If the server's event loop blocks
while streaming the big file (the bug fixed on this branch), probe
latency spikes; if it stays non-blocking, probes stay fast regardless.

Usage:
    ./webserver test.config &
    python3 tests/manual/load_test.py
"""
import argparse
import hashlib
import os
import sys

from httpload import run_workers, start_workers, stop_and_report

DEFAULT_BIG_PATH = "/loadtest/bigfile.bin"
BIG_FILE_REL = "www/loadtest/bigfile.bin"


def ensure_big_file(project_root, size_mb):
    """Create (if needed) the big fixture file and return (path, sha256_hex)."""
    path = os.path.join(project_root, BIG_FILE_REL)
    size = size_mb * 1024 * 1024
    if not (os.path.exists(path) and os.path.getsize(path) == size):
        os.makedirs(os.path.dirname(path), exist_ok=True)
        chunk = os.urandom(1024 * 1024)
        with open(path, "wb") as f:
            written = 0
            while written < size:
                f.write(chunk)
                written += len(chunk)
    hasher = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(1024 * 1024), b""):
            hasher.update(chunk)
    return path, hasher.hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--host", default="localhost")
    parser.add_argument("--port", type=int, default=8080)
    parser.add_argument("--project-root", default=os.path.join(os.path.dirname(__file__), "..", ".."),
                         help="used to create the big test file under www/")
    parser.add_argument("--big-path", default=DEFAULT_BIG_PATH)
    parser.add_argument("--big-size-mb", type=int, default=50)
    parser.add_argument("--small-path", default="/index.html")
    parser.add_argument("--slow-clients", type=int, default=3)
    parser.add_argument("--slow-chunk", type=int, default=4096)
    parser.add_argument("--slow-delay", type=float, default=0.02, help="sleep (s) between reads for slow clients")
    parser.add_argument("--probes", type=int, default=5, help="concurrent probe workers hitting --small-path")
    parser.add_argument("--probe-interval", type=float, default=0.05)
    parser.add_argument("--timeout", type=float, default=10.0)
    parser.add_argument("--duration", type=float, default=15.0)
    parser.add_argument("--fail-threshold-ms", type=float, default=250.0,
                         help="probe latency above this while big downloads run is treated as a stall")
    parser.add_argument("--skip-baseline", action="store_true")
    args = parser.parse_args()

    _, big_sha256 = ensure_big_file(args.project_root, args.big_size_mb)

    if not args.skip_baseline:
        print(f"Baseline: probing {args.small_path} with no concurrent big downloads...")
        run_workers(args.host, args.port, args.small_path, min(3.0, args.duration),
                    args.probes, args.timeout, "baseline (idle)", interval=args.probe_interval)

    print(f"\nStarting {args.slow_clients} slow downloader(s) of {args.big_path} "
          f"({args.big_size_mb} MB, {args.slow_delay * 1000:.0f}ms/{args.slow_chunk}B) "
          f"for {args.duration}s, probing {args.small_path} concurrently...")

    dl_stop, dl_threads, dl_results = start_workers(
        args.host, args.port, args.big_path, args.slow_clients, args.timeout,
        read_chunk=args.slow_chunk, pace_delay=args.slow_delay, expected_sha256=big_sha256)

    samples_ms, errors, _ = run_workers(args.host, args.port, args.small_path, args.duration,
                                         args.probes, args.timeout, "under load (slow big-file downloads)",
                                         interval=args.probe_interval)

    _, _, dl_details = stop_and_report(dl_stop, dl_threads, dl_results, args.timeout, "slow big-file downloads")

    corrupted = dl_details.get("checksum_mismatch", 0)
    if corrupted:
        print(f"\nFAIL: {corrupted} big-file download(s) had a checksum mismatch — "
              f"data corruption under concurrent load.")
        sys.exit(1)

    if not samples_ms:
        print("\nFAIL: no successful probe requests captured.")
        sys.exit(1)

    worst = max(samples_ms)
    if worst > args.fail_threshold_ms or errors:
        print(f"\nFAIL: probe latency spiked to {worst:.1f}ms (threshold {args.fail_threshold_ms}ms) "
              f"or requests failed ({errors}) while the big file was being served — "
              f"event loop may be blocking.")
        sys.exit(1)

    print(f"\nPASS: probes stayed responsive (max {worst:.1f}ms) while the big file was being served.")


if __name__ == "__main__":
    main()
