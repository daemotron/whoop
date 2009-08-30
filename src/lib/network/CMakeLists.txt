include_directories(${whoop_SOURCE_DIR}/src/include ${whoop_BINARY_DIR}/src/include)

add_library(network readline.c readn.c writen.c tcp_connect.c tcp_listen.c)
