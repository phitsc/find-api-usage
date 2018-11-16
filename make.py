#!/usr/bin/env python3

"""
Builds find-api-usage within a Docker container
if a suitable Docker image named libtooling exists.

This script performs the following actions:

1. Start a Docker container from the 'libtooling' image that contains
   everything required to build LLVM, Clang and the Clang extra tools.
2. Map the current directory as an additional extra tool into the container.
3. Start the docker-build.sh script that builds LLVM, Clang and the extra tools.
4. On successful completion of the build, commit the current state of the container
   into an image named PROJECT_NAME that can then be started to run the
   built application.
"""

from pathlib import Path
from subprocess import check_call
from helpers import docker_image_exists, docker_container_exists, fix_wsl_path

PROJECT_NAME = "find-api-usage"
BUILD_IMAGE_NAME = "phitsc/libtooling-travis"
BUILD_CONTAINER_NAME = "{}_build".format(PROJECT_NAME)


def main():
    """ The main function """
    if docker_image_exists(BUILD_IMAGE_NAME):
        if docker_container_exists(BUILD_CONTAINER_NAME):
            check_call(["docker", "start", "-i", BUILD_CONTAINER_NAME])
        else:
            working_dir = fix_wsl_path(Path.cwd())

            check_call(
                [
                    "docker",
                    "run",
                    "-it",
                    "--name",
                    BUILD_CONTAINER_NAME,
                    "-v",
                    "{}:/root/clang-llvm/llvm/tools/clang/tools/extra/{}".format(
                        working_dir.as_posix(), PROJECT_NAME
                    ),
                    BUILD_IMAGE_NAME,
                    # pylint: disable=line-too-long
                    "/root/clang-llvm/llvm/tools/clang/tools/extra/{}/Docker/docker-build.sh".format(
                        PROJECT_NAME
                    )
                    #'--entrypoint', f"/bin/bash",
                ]
            )

        print("Creating Docker image {}".format(PROJECT_NAME))
        check_call(["docker", "commit", BUILD_CONTAINER_NAME, PROJECT_NAME])
    else:
        raise RuntimeError(
            "{} Docker image not found. Run 'docker build -t {} .' first.".format(
                BUILD_IMAGE_NAME, BUILD_IMAGE_NAME
            )
        )


if __name__ == "__main__":
    try:
        main()
    except RuntimeError as error:
        print("Error:", error)
