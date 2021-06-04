#!/usr/bin/env python3

import sys

from testsupport import info, run_project_executable, warn, run, subprocess


def main() -> None:
    # Replace with the executable you want to test
    with open("server.txt", "w+") as stdout:
        try:
            info("Run multithreaded-server test (2 threads) ...")
            threads = "2"
            port    = "1025"
            pro = run_project_executable("server", args=[threads, port], stdout=stdout, timeout=10)


        except subprocess.TimeoutExpired as e:
            output = open("server.txt").readlines()
            output2 = open("client.txt").readlines()
#            print (output)
#           print (output2)
            output.sort();
            print(output)
            output2.sort();
            print(output2)
            if output != output2:
                warn(f"output does not match")
                sys.exit(2)

            info("OK: we killed server process")

        except OSError as e:
            warn(f"Failed to run command: {e}")
            sys.exit(1)


if __name__ == "__main__":
    main()
