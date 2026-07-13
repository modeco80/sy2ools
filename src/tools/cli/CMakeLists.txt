add_subdirectory(slyarc)

add_executable(brxdecompress
	brxdecompress.cpp
)
sy2ools_target(brxdecompress)
target_link_libraries(brxdecompress
	libsly::core
)

# "test harness" for the brx parser
add_executable(brxtest
	brxtest.cpp
)
sy2ools_target(brxtest)
target_link_libraries(brxtest
	libsly::core
	libsly::sly2
)
