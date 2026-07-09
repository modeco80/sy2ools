add_library(libsly_core
	buffered_read_stream.cpp
	buffered_write_stream.cpp

	sly_lz.cpp
	sly_lz_stream.cpp
)

target_link_libraries(libsly_core PUBLIC
	mco::base
	mco::io
)

sy2ools_target(libsly_core)
add_library(libsly::core ALIAS libsly_core)
