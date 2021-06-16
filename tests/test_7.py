#!/usr/bin/env python3

import os
import sys

from testsupport import info, run_project_executable, warn, run, subprocess, find_project_executable, find_library


def main() -> None:
    # Replace with the executable you want to test
    with open("client_output_test_7.txt", "w+") as stdout:
        try:
            cmd = find_project_executable("server")
            print(cmd)
            lib = find_library("libutil.so")
            env = {"LD_LIBRARY_PATH": str(os.path.dirname(lib))} if lib else {}
            info("Run multithreaded-server test (2 threads) ...")
            with subprocess.Popen([cmd, "2", "1025"], stdout=subprocess.PIPE, env=env) as proc:
                info("Run multithreaded-client(s) ...")
                run_project_executable("client", args=["24", "localhost", "1025", "5000"], stdout=stdout, extra_env=env)
                run_project_executable("client", args=["24", "localhost", "1025", "5000"], stdout=stdout, extra_env=env)
                run_project_executable("client", args=["240", "localhost", "1025", "5000"], stdout=stdout, extra_env=env)
                run_project_executable("client", args=["1", "localhost", "1025", "5000"], stdout=stdout, extra_env=env)
                run_project_executable("client", args=["10", "localhost", "1025", "5000"], stdout=stdout, extra_env=env)
                run_project_executable("client", args=["10", "localhost", "1025", "5000"], stdout=stdout, extra_env=env)
                run_project_executable("client", args=["10", "localhost", "1025", "5000"], stdout=stdout, extra_env=env)
                run_project_executable("client", args=["10", "localhost", "1025", "5000"], stdout=stdout, extra_env=env)
                run_project_executable("client", args=["10", "localhost", "1025", "5000"], stdout=stdout, extra_env=env)

                proc.kill();
                outs, errs = proc.communicate()

                info("OK")

        except OSError as e:
            warn(f"Failed to run command: {e}")
            sys.exit(1)


if __name__ == "__main__":
    main()
