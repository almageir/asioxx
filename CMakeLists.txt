cmake_minimum_required(VERSION 3.21)

project(asioxx LANGUAGES CXX)

option(ASIOXX_BUILD_EXAMPLES "Build examples" OFF)

add_subdirectory(dataio)
add_subdirectory(asynclog)

if (ASIOXX_BUILD_EXAMPLES)
    add_subdirectory(examples/logger_demo)
endif()

