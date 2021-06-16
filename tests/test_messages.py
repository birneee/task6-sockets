#!/usr/bin/env python3

import os
import sys

from testsupport import info, run_project_executable, warn, find_library


def main() -> None:
    # Replace with the executable you want to test
    try:
        info("Run message format tests ...")
        lib = find_library("libutil.so")
        env = {"LD_LIBRARY_PATH": str(os.path.abspath(os.path.dirname(lib)))} if lib else {}
        run_project_executable("message_formatting_tests", extra_env=env)
        info("OK")
    except OSError as e:
        warn(f"Failed to run command: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
