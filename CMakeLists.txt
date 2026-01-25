cmake_minimum_required(VERSION 3.21)

project(asioxx LANGUAGES CXX)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set(CMAKE_VS_JUST_MY_CODE_DEBUGGING "$<$<CONFIG:Debug>:ON>")
endif()

add_subdirectory(dataio)
add_subdirectory(asynclog)
add_subdirectory(examples/logger_demo)

