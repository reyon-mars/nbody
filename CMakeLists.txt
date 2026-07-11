cmake_minimum_required(VERSION 3.20)

project(nbody LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(Catch2)


add_library(nbody_core INTERFACE)
target_include_directories(nbody_core INTERFACE include)


add_executable(nbody_tests tests/test_vector3d.cpp)
target_link_libraries(nbody_tests PRIVATE
    nbody_core
    Catch2::Catch2WithMain
)

if(MSVC)
    target_compile_options(nbody_tests PRIVATE /w4)
else()
    target_compile_options(nbody_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()
