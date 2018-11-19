#!/usr/bin/env bash

set -e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

# set project_name variable
source ${DIR}/project-name.sh

unittests_dir="/root/clang-llvm/llvm/tools/clang/tools/extra/${project_name}/test/unit_tests"

if [ ! -d "${unittests_dir}" ]
then
    echo "Run this script only within a libtooling docker container."
    exit 1
fi

unittests_build_dir=${unittests_dir}/builddir

if [ ! -d "${unittests_build_dir}" ]
then
    mkdir ${unittests_build_dir}
fi

cd ${unittests_build_dir}

echo "Building unit tests for ${project_name}"

cmake \
    -G Ninja \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_USE_LINKER=gold \
    .. \
    && ninja -j12
