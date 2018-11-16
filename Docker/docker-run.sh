#!/usr/bin/env bash

set -e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

# set project_name variable
source ${DIR}/project-name.sh

exe_name=$project_name

if [ -f /root/clang-llvm/build/bin/${exe_name} ]
then
    echo "/root/clang-llvm/build/bin/${exe_name} $@"
    /root/clang-llvm/build/bin/${exe_name} $@
else
    echo "Error: ${exe_name} not found. Call make.py first."
fi
