#!/usr/bin/env python3

import argparse
from pathlib import Path
from subprocess import call
from helpers import resolve_path

PROJECT_NAME = "find-api-usage"
TEST_PROJECT_NAME = "integration_tests"


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--json",
        help="Export test results to JSON file (in builddir)",
        action="store_true",
    )
    parser.add_argument(
        "--diff",
        help="Diff exported JSON file against the one in ./json",
        action="store_true",
    )
    parser.add_argument(
        "-v", "--verbose",
        help="Be verbose when reporting violations",
        action="store_true",
    )
    args = parser.parse_args()

    if args.json or args.diff:
        jsonFile = "/root/test_project/{}/builddir/{}_results.json".format(
            TEST_PROJECT_NAME, PROJECT_NAME)
    else:
        jsonFile = None

    call_args = (
        ["python3", "scripts/run.py"]
        + ([("-json " + jsonFile)] if jsonFile else [])
        + (["-v"] if args.verbose else [])
        + [ "-fc", "TestClass::fn2" ]
        + [
            "-p",
            str(resolve_path(Path("test/{}/build".format(TEST_PROJECT_NAME)))),
            str(resolve_path(Path("test/{}/src/main.cpp".format(TEST_PROJECT_NAME)))),
        ]
    )

    call(call_args)

    if args.diff:
        call(
            [
                "diff",
                str(
                    resolve_path(
                        Path("test/{}/builddir/{}_results.json".format(
                            TEST_PROJECT_NAME, PROJECT_NAME))
                    )
                ),
                str(
                    resolve_path(Path("test/{}/json/{}_results.json".format(
                        TEST_PROJECT_NAME, PROJECT_NAME)))
                ),
            ]
        )


if __name__ == "__main__":
    try:
        main()
    except RuntimeError as error:
        print("Error:", error)
