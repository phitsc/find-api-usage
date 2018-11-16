FROM phitsc/libtooling-travis:latest

ENV CC=clang \
    CXX=clang++

WORKDIR /root/clang-llvm/build
