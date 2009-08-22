include_directories(${whoop_SOURCE_DIR}/src/include ${whoop_BINARY_DIR}/src/include)
link_directories(${whoop_SOURCE_DIR}/src/lib/daemon)

add_executable(daemon-test daemon-test.c)
target_link_libraries(daemon-test daemon)
