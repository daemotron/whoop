# basic project settings
cmake_minimum_required (VERSION 2.6)
project (whoop)

# compiler settings
set(CMAKE_X_FLAGS "-O3 -Wall")

add_subdirectory(src)
