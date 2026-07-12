add_library(libsly_sly2
	archive_filesystem.cpp

	release.cpp
	iso_filemapping.cpp
	iso_filesystem.cpp

	# brx
	brx/parser.cpp
)
target_link_libraries(libsly_sly2 PUBLIC
	mco::base
	mco::io
	libsly::core
)

sy2ools_target(libsly_sly2)
add_library(libsly::sly2 ALIAS libsly_sly2)
