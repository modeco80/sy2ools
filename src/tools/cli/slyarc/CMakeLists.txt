
add_executable(slyarc
	main.cpp
	utils.cpp
	cmd.cpp

	# commands
	cmd_list.cpp
	cmd_extract_all.cpp
)
sy2ools_target(slyarc)
target_link_libraries(slyarc
	libsly::core
	libsly::sly2
)
