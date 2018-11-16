cmake_minimum_required(VERSION 3.11)

enable_testing()

set(TARGET test-find-api-usage)

project(${TARGET})

add_executable(${TARGET}
	src/main.cpp
    src/TestHelpers.cpp
)

target_compile_features(${TARGET}
    PRIVATE cxx_std_17)

target_include_directories(${TARGET}
    PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../../src"
)

add_test(unittests ${TARGET})
