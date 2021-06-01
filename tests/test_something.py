#!/usr/bin/env python3

import sys

from testsupport import info, run_project_executable, warn


def main() -> None:
    # Replace with the executable you want to test
    try:
        info("Run message_formatting_test ...")
        run_project_executable("message_formatting_test")
        info("OK")
    except OSError as e:
        warn(f"Failed to run command: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
