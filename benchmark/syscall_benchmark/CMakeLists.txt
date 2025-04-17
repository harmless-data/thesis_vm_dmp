cmake_minimum_required(VERSION 3.10)

project(syscall_benchmark)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2")

set(CMAKE_BUILD_TYPE "RelWithDebInfo")

add_executable(syscall_benchmark 
    main.cpp
    SyscallBenchmark.h
    SyscallBenchmark.cpp
)
