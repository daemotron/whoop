include(CheckLibraryExists)

include_directories(
	${whoop_SOURCE_DIR}/src/include 
	${whoop_BINARY_DIR}/src/include
)

link_directories(
	${whoop_SOURCE_DIR}/src/lib/barrier
	${whoop_SOURCE_DIR}/src/lib/daemon
	${whoop_SOURCE_DIR}/src/lib/msg
	${whoop_SOURCE_DIR}/src/lib/network
)

add_executable(test-client test-client.c)

check_library_exists(socket freeaddrinfo "" HAVE_SOCKET_L)

if (HAVE_SOCKET_L)
	target_link_libraries(test-client barrier msg network pthread socket)
else (HAVE_SOCKET_L)
	target_link_libraries(test-client barrier msg network pthread)
endif (HAVE_SOCKET_L)

