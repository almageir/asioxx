cmake_minimum_required(VERSION 3.21)

project(logger_demo LANGUAGES CXX)

add_executable(logger_demo)

target_sources(logger_demo PRIVATE main.cpp)
target_link_libraries(logger_demo PRIVATE asynclog)
target_compile_features(logger_demo PRIVATE cxx_std_17)

# Linux static linking
#target_link_options(logger_demo PRIVATE -static)
