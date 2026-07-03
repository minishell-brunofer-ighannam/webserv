#!/usr/bin/env python3
"""CGI timeout / non-blocking regression test for webserv.

Fires requests at a CGI script that sleeps far longer than the server's CGI
timeout (30s, hardcoded in ConnectionPool.hpp) while concurrently probing a
static path. Verifies:
  1. the server's CGI timeout actually fires (504, not a hang forever)
  2. other clients stay responsive while the CGI is stuck - the same kind of
     event-loop-blocking risk as the big-file bug (load_test.py), but on the
     CGI/pipe path instead of the sendfile path.

Requires www/cgi-bin/stuck.py, already in the repo, which sleeps 60s.

Usage:
    ./webserver test.config &
    python3 tests/manual/cgi_hang_test.py
"""
import argparse
import sys

from httpload import run_workers, start_workers, stop_and_report

CGI_TIMEOUT_S = 30  # matches the hardcoded timeout in ConnectionPool.hpp
STATUS_504 = (b"HTTP/1.1 504", b"HTTP/1.0 504")


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--host", default="localhost")
    parser.add_argument("--port", type=int, default=8080)
    parser.add_argument("--hang-path", default="/cgi-bin/stuck.py")
    parser.add_argument("--small-path", default="/index.html")
    parser.add_argument("--hang-clients", type=int, default=3)
    parser.add_argument("--probes", type=int, default=5)
    parser.add_argument("--probe-interval", type=float, default=0.05)
    parser.add_argument("--duration", type=float, default=CGI_TIMEOUT_S + 10,
                         help="must exceed the server's CGI timeout so the 504 has time to arrive")
    parser.add_argument("--timeout", type=float, default=CGI_TIMEOUT_S + 15,
                         help="client socket timeout; must comfortably exceed the CGI timeout")
    parser.add_argument("--fail-threshold-ms", type=float, default=250.0,
                         help="probe latency above this while CGIs are stuck is treated as a stall")
    args = parser.parse_args()

    if args.duration <= CGI_TIMEOUT_S:
        parser.error(f"--duration must be greater than the server's CGI timeout ({CGI_TIMEOUT_S}s)")

    print(f"Starting {args.hang_clients} client(s) hitting {args.hang_path} "
          f"(expected to hang ~{CGI_TIMEOUT_S}s then 504), probing {args.small_path} "
          f"concurrently for {args.duration}s...")

    # max_iterations=1: each client makes exactly one ~30s request and stops,
    # so there's no risk of a second in-flight call getting cut off awkwardly
    # when we tear things down below.
    hang_stop, hang_threads, hang_results = start_workers(
        args.host, args.port, args.hang_path, args.hang_clients, args.timeout,
        ok_status_prefixes=STATUS_504, max_iterations=1)

    samples_ms, errors, _ = run_workers(args.host, args.port, args.small_path, args.duration,
                                         args.probes, args.timeout, "under load (stuck CGI)",
                                         interval=args.probe_interval)

    hang_samples_ms, _, hang_details = stop_and_report(
        hang_stop, hang_threads, hang_results, args.timeout, "stuck CGI requests")

    ok = True

    if not hang_samples_ms:
        print(f"\nFAIL: no stuck CGI request returned a 504 within {args.duration}s — "
              f"the timeout may not be firing (server could hang forever on a stuck CGI). "
              f"detail: {hang_details}")
        ok = False
    else:
        fastest_s = min(hang_samples_ms) / 1000.0
        if fastest_s < CGI_TIMEOUT_S - 1:
            print(f"\nFAIL: a 504 arrived after only {fastest_s:.1f}s, "
                  f"before the expected {CGI_TIMEOUT_S}s CGI timeout — unexpected early failure.")
            ok = False

    if not samples_ms:
        print("\nFAIL: no successful probe requests captured while the CGI was stuck.")
        ok = False
    elif max(samples_ms) > args.fail_threshold_ms or errors:
        print(f"\nFAIL: probe latency spiked to {max(samples_ms):.1f}ms "
              f"(threshold {args.fail_threshold_ms}ms) or requests failed ({errors}) "
              f"while the CGI was stuck — event loop may be blocking on the CGI pipe.")
        ok = False

    if not ok:
        sys.exit(1)

    print(f"\nPASS: stuck CGI(s) correctly timed out with 504 "
          f"({min(hang_samples_ms) / 1000:.1f}-{max(hang_samples_ms) / 1000:.1f}s) and probes "
          f"stayed responsive (max {max(samples_ms):.1f}ms) throughout.")


if __name__ == "__main__":
    main()
