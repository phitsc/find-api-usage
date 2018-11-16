#!/usr/bin/env python3

"""
Runs find-api-usage within a Docker container
if a suitable Docker image named find-api-usage exists.

This script performs the following actains:

1. Start a Docker container from the PROJECT_NAME Docker image.
2. Map the current directory and the parent directory of the
   directory passed with the -p option into the container.
3. Fix any paths that are passed to the tool so that they
   point to their respective paths within the container.
   In particular, update the paths in the compile_commands.json
   file so that they represent valid paths within the container.
5. Start the docker-run.sh script that executes the find-api-usage
   tool in the container, passing on any command line arguments
   that have been supplied on the command line.
6. Restore the compile_commands.json file to its original state.
"""

import argparse
import fileinput

from os import remove, rename
from pathlib import Path
from subprocess import call
from helpers import docker_image_exists, fix_wsl_path, resolve_path

PROJECT_NAME = "find-api-usage"


def redirect_paths(unknown_args, name, directory):
    """ For every argument in unknown_args which is an existing
        file path containing name, the file path is redirected
        to directory"""
    ret = []

    for arg in unknown_args:
        if Path(arg).exists():
            path = str(fix_wsl_path(resolve_path(Path(arg))))
            pos = path.find(name)
            if pos >= 0:
                ret.append(directory + path[pos + len(name) :])
            else:
                ret.append(path)
        else:
            ret.append(arg)

    return ret


# pylint: disable=C0330
def patch_compile_commands(
    compile_commands_file, test_project_dir, container_project_dir
):
    """ Replaces occurrences of test_project_dir in the compilation
        database file with container_project_dir. Also replaces \
        by / """
    with fileinput.FileInput(
        str(compile_commands_file), inplace=True, backup=".bak"
    ) as file:
        for line in file:
            print(
                line.replace("\\\\", "/").replace(
                    test_project_dir, container_project_dir
                ),
                end="",
            )


def main():
    """ The main function """
    if docker_image_exists(PROJECT_NAME):

        # extract test project name and directory from -p argument
        parser = argparse.ArgumentParser(add_help=False)
        parser.add_argument("-p")
        args, unknown_args = parser.parse_known_args()

        if args.p:
            test_project_build_dir = Path(args.p).resolve()
            test_project_dir = test_project_build_dir.parent
            container_project_dir = "/root/test_project/" + test_project_dir.stem
            compile_commands_file = Path(args.p) / "compile_commands.json"
        else:
            test_project_build_dir = None
            test_project_dir = None
            container_project_dir = None
            compile_commands_file = None

        # redirect local paths to their respective directory within the container
        if container_project_dir:
            unknown_args = redirect_paths(
                unknown_args, test_project_dir.stem, container_project_dir
            )

        # project_volume required to run docker-run.sh script
        project_volume = "{}:/root/clang-llvm/llvm/tools/clang/tools/extra/{}".format(
            fix_wsl_path(Path.cwd()), PROJECT_NAME
        )

        # test project volume and arguments
        if test_project_dir:
            test_project_volume = "{}:{}".format(
                fix_wsl_path(test_project_dir), container_project_dir
            )

            test_project_args = "-p /root/test_project/{}/{} {}".format(
                test_project_dir.stem,
                test_project_build_dir.stem,
                " ".join(unknown_args),
            )
        else:
            # if -p was not supplied, just pass same volume twice
            # to specify a valid volume
            test_project_volume = project_volume
            test_project_args = " ".join(unknown_args)

        if compile_commands_file and compile_commands_file.exists:
            patch_compile_commands(
                compile_commands_file,
                test_project_dir.as_posix(),
                container_project_dir,
            )

        call(
            [
                "docker",
                "run",
                "-it",  # use container interactively
                "--rm",  # remove container when it exits
                "-v",
                project_volume,
                "-v",
                test_project_volume,
                PROJECT_NAME,  # project name = image name
                "/root/clang-llvm/llvm/tools/clang/tools/extra/{}/Docker/docker-run.sh".format(
                    PROJECT_NAME
                ),
                test_project_args  # pass on command line arguments
                # "/bin/bash"
            ]
        )

        if compile_commands_file and compile_commands_file.exists:
            remove(str(compile_commands_file))
            rename(
                str(compile_commands_file.with_suffix(".json.bak")),
                str(compile_commands_file),
            )

    else:
        raise RuntimeError(
            "{} Docker image not found. Run make.py first.".format(PROJECT_NAME)
        )


if __name__ == "__main__":
    try:
        main()
    except RuntimeError as error:
        print("Error:", error)
