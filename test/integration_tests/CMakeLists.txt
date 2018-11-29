cmake_minimum_required(VERSION 3.11)

project(test-find-api-usage)

add_executable(test-find-api-usage
	src/main.cpp
)

target_compile_features(test-find-api-usage
    PRIVATE cxx_std_17)
