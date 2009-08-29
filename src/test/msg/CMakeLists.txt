include_directories(
	${whoop_SOURCE_DIR}/src/include 
	${whoop_BINARY_DIR}/src/include
)

link_directories(
	${whoop_SOURCE_DIR}/src/lib/msg
)

add_executable(msg-test msg-test.c)
target_link_libraries(msg-test msg)
