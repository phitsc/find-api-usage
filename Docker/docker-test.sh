#!/usr/bin/env bash

set -e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

# set project_name variable
source ${DIR}/project-name.sh

# Build and run the unit tests
source ${DIR}/docker-build-unittests.sh

echo "Running unit tests for ${project_name}"

${unittests_build_dir}/test-${project_name}

# Build the application
source ${DIR}/docker-build.sh

# Run the integration tests
test_dir=/root/test_project
mkdir -p ${test_dir}

test_project_dir=${test_dir}/project1
ln -sfn ${llvm_root_dir}/llvm/tools/clang/tools/extra/${project_name}/test/project1 ${test_project_dir}

# echo "Running integration tests for ${project_name}"

# ${llvm_build_dir}/bin/${project_name} \
# 	-json ${llvm_build_dir}/${project_name}_results.json \
# 	-p ${test_project_dir}/build/ \
# 	${test_project_dir}/src/TestClass.cpp

# diff ${llvm_build_dir}/${project_name}_results.json ${test_project_dir}/json/${project_name}_results.json
