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
target_link_libraries(test-client barrier msg network pthread)
