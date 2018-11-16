cmake_minimum_required(VERSION 3.11)

set(LLVM_LINK_COMPONENTS support)

set(TARGET find-api-usage)

add_clang_executable(${TARGET}
    src/main.cpp
)

target_link_libraries(${TARGET} PRIVATE
    clangTooling
    clangBasic
    clangASTMatchers
    stdc++fs
)

target_compile_definitions(${TARGET}
    PRIVATE CLANG_BUILTIN_INCLUDES_DIR="${CLANG_BUILTIN_INCLUDES_DIR}")

target_compile_features(${TARGET}
    PRIVATE cxx_std_17)

set (CMAKE_EXE_LINKER_FLAGS "-static-libstdc++")
