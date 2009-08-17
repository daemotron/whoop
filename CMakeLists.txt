# basic project settings
cmake_minimum_required (VERSION 2.6)
project (whoop)

# compiler settings
set(CMAKE_BUILD_TYPE whoop)
set(CMAKE_C_FLAGS_WHOOP "-O3 -Wall")
set(CMAKE_CXX_FLAGS_WHOOP "-O3 -Wall")

add_subdirectory(src)
