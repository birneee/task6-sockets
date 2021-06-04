#!/usr/bin/env python3

import sys

from testsupport import info, run_project_executable, warn


def main() -> None:
    # Replace with the executable you want to test
    try:
        with open("client.txt", "w+") as stdout:
            info("Run multithreaded clients (24 threads) ...")
            run_project_executable("client", args=["24", "localhost", "1025", "5000"], stdout=stdout)
            info("OK")

    except OSError as e:
        warn(f"Failed to run command: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
