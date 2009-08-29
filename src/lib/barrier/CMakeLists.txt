include_directories(${whoop_SOURCE_DIR}/src/include ${whoop_BINARY_DIR}/src/include)

add_library(barrier barrier_init.c barrier_destroy.c barrier_wait.c)
