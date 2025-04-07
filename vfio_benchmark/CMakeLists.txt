cmake_minimum_required(VERSION 3.10)

project(vfio_benchmark)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(vfio_benchmark 
    main.cpp
    VFIOBenchmark.h
    VFIOBenchmark.cpp
    )

