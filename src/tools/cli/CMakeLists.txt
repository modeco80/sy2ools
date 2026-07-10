
add_executable(sly2_iso_pull_files
	sly2_iso_pull_files.cpp
)
sy2ools_target(sly2_iso_pull_files)
target_link_libraries(sly2_iso_pull_files
	libsly::core
	libsly::sly2
)

add_executable(brxdecompress
	brxdecompress.cpp
)
sy2ools_target(brxdecompress)
target_link_libraries(brxdecompress
	libsly::core
)
