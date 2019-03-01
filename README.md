# find-api-usage

[![Build Status](https://travis-ci.com/phitsc/find-api-usage.svg?token=RqqkynutptY9gpoo5YZP&branch=master)](https://travis-ci.com/phitsc/find-api-usage)

## Try out find-api-usage without building

*find-api-usage* can be tried out on the libbitcoin libraries using the *phitsc/check-libbitcoin-api* Docker image. On any system with Docker installed, run the following command to start a Bash shell in a Docker container based on the *phitsc/check-libbitcoin-api* Docker image.

        docker run -it --rm phitsc/check-libbitcoin-api:1.3 /bin/bash

Then, within the Bash shell in the newly started Docker container, run for example:

        cd ~/libbitcoin-all
        find . -name '*.cpp' -exec find-api-usage \
            -function-call='create_key_pair' \
            -function-call='create_token' \
            -function-call='encrypt' \
            -function-call='sign_message' \
            {} + 2>/dev/null

## Build instructions

### Option 1: Build together with Clang on Linux

1. Follow the build instructions on https://clang.llvm.org/get_started.html . Make sure to include step 4 'extra Clang tools'.

2. Clone the *find-api-usage* repository into `llvm/tools/clang/tools/extra/`.

3. Add `add_subdirectory(find-api-usage)` at the bottom of `llvm/tools/clang/tools/extra/CMakeLists.txt`

4. Build everything as described in step 7 'Build LLVM and Clang' on https://clang.llvm.org/get_started.html .

### Option 2: Build with Docker

1. Clone the *find-api-usage* repository into a directory of your choice and change into that directory.

        git clone --recursive https://github.com/phitsc/find-api-usage.git
        cd find-api-usage

2. Create the *libtooling* docker image for building Clang tools.

        sudo docker build -t libtooling api-utils-common/Docker

3. Run the make script.

        ./make

## Running find-api-usage against the test project

1. Change into `find-api-usage/test/integration_tests`.

        cd test/integration_tests

2. Create the `builddir` directory and change into it.

        mkdir builddir
        cd builddir

3. Create the compilation database.

        cmake -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..

    Make sure the `compile_commands.json` file has been created in the `integration_tests/builddir` directory.

4. Change back into the `find-api-usage` directory and run *find-api-usage*.

        cd ../../..
        ./find-api-usage \
            -p test/integration_tests/builddir \
            -function-call='fn2'
            -extra-arg=-Wno-unused-variable
            test/integration_tests/src/main.cpp

    Note that *find-api-usage* must be executed on the same host platform on which the compilation database was created or the script will not be able to map the source file paths from the host filesystem to to container filesystem.


