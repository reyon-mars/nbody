# ---------------------------------------------------------------------
# CMAKE (ROOT)
# ---------------------------------------------------------------------
cmake_minimum_required(VERSION 3.20)

project(nbody LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# *********************************************************************
# External Dependencies
# *********************************************************************
# -> Catch2
# -> Google Benchmark
# *********************************************************************

option(BUILD_TESTING "Build the project tests" ON)
if(BUILD_TESTING)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.4.0
    )
    FetchContent_MakeAvailable(Catch2)
endif()

option(BUILD_BENCHMARKS "Build the project benchmarks" ON)
if(BUILD_BENCHMARKS)
    find_package(Threads REQUIRED)

    FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.8.3
    )
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googlebenchmark)
endif()

# *********************************************************************
# Core Library 
# *********************************************************************

add_library(nbody_core INTERFACE)
target_include_directories(nbody_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# *********************************************************************
# Subdirectories
# *********************************************************************
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

if(BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

# ---------------------------------------------------------------------
# This is the END  
# ---------------------------------------------------------------------
