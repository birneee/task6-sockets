#!/usr/bin/env python3

import sys

from testsupport import info, run_project_executable, warn, run


def main() -> None:
    # Replace with the executable you want to test
    try:
        with open("server.txt", "w+") as stdout:
            info("Run single-threaded server test ...")
            port = "1025"
            run_project_executable("server_test", args=[port], stdout=stdout)

            info("OK")
            with open("output_.txt", "w+") as output_:
                run(["diff", "-q", "server.txt", "client.txt"], stdout=output_)
                output = open("output_.txt").readlines()
                if len(output) == 0:
                    info("OK")
                else:
                    warn(f"Failed \n")
                    sys.exit(1)

    except OSError as e:
        warn(f"Failed to run command: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
