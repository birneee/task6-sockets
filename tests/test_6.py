#!/usr/bin/env python3

import sys

from testsupport import info, run_project_executable, warn, run, subprocess, find_project_executable


def main() -> None:
    # Replace with the executable you want to test
    with open("client_output_test_6.txt", "w+") as stdout:
        try:
            cmd = find_project_executable("server")
            print(cmd)
            info("Run multithreaded-server test (2 threads) ...")
            with subprocess.Popen([cmd, "2", "1025"], stdout=subprocess.PIPE) as proc:
                info("Run multithreaded-client test (24 threads) ...")
                run_project_executable("client", args=["24", "localhost", "1025", "5000"], stdout=stdout)
                proc.kill();
                outs, errs = proc.communicate()

                Lista = [x for x in outs.decode('utf-8').replace('\\n', '\n').split('\n') if x!='']
                Lista.sort()
                print(Lista)

                output = open("client_output_test_6.txt").readlines()
                Listb =  [x.replace('\n', '') for x in output if x!='']
                Listb.sort()
                if Lista != Listb:
                    warn(f"output does not match")
                    print(Lista)
                    print(Listb)
                    sys.exit(2)
                info("OK")

        except OSError as e:
            warn(f"Failed to run command: {e}")
            sys.exit(1)


if __name__ == "__main__":
    main()