#!/usr/bin/env python3

import sys

from testsupport import info, run_project_executable, warn, run, subprocess


def main() -> None:
    # Replace with the executable you want to test
    try:
        with open("server.txt", "w+") as stdout:
            info("Run server_test ...")
            threads = "2"
            port    = "1025"
            pro = run_project_executable("server", args=[threads, port], stdout=stdout, timeout=10)


    except subprocess.TimeoutExpired as e:
        info("OK: we killed server process")

    except OSError as e:
        warn(f"Failed to run command: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
