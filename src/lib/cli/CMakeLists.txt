include_directories(${whoop_SOURCE_DIR}/src/include ${whoop_BINARY_DIR}/src/include)

add_library(cli cli.c new.c init.c help.c version.c)
