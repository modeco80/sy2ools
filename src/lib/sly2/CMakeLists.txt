add_library(libsly_sly2
	# core archive/file stuff
	archive_filesystem.cpp
	file_location.cpp
	release.cpp
	fk_utils.cpp

	# iso filesystem
	iso_filemapping.cpp
	iso_filesystem.cpp

	# brx
	brx/io_utils.cpp
	brx/parser.cpp
	brx/options.cpp
	brx/option.cpp
)
target_link_libraries(libsly_sly2 PUBLIC
	mco::base
	mco::io
	libsly::core
)

sy2ools_target(libsly_sly2)
add_library(libsly::sly2 ALIAS libsly_sly2)
