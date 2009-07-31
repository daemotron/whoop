include_directories(${whoop_SOURCE_DIR}/src/include ${whoop_BINARY_DIR}/src/include)
link_directories(${whoop_SOURCE_DIR}/src/lib/cli)

add_executable(whoop whoop.c)
target_link_libraries(whoop cli)
