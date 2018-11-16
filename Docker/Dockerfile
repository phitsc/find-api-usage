FROM fedora:latest

RUN dnf install -y \
    binutils \
    boost-devel \
    clang \
    cmake \
    git \
    libstdc++-static \
    ninja-build \
    python

ENV CC=clang \
    CXX=clang++

WORKDIR /root/clang-llvm

RUN git clone https://git.llvm.org/git/llvm.git

WORKDIR /root/clang-llvm/llvm/tools

RUN git clone https://git.llvm.org/git/clang.git

WORKDIR /root/clang-llvm/llvm/tools/clang/tools

RUN git clone https://git.llvm.org/git/clang-tools-extra.git extra

WORKDIR /root/clang-llvm/build
