add_library(libsly_core
	buffered_read_stream.cpp
	buffered_write_stream.cpp

	sly_lz.cpp
	sly_lz_stream.cpp
)

if(LIBSLY_USE_OPENSSL)
	find_package(OpenSSL CONFIG REQUIRED)
	target_link_libraries(libsly_core PRIVATE
		OpenSSL::Crypto
	)
	target_sources(libsly_core PRIVATE
		sha256_openssl.cpp
	)
# TODO Else: Standalone or other crypto libraries?
endif()

target_link_libraries(libsly_core PUBLIC
	mco::base
	mco::io
)

sy2ools_target(libsly_core)
add_library(libsly::core ALIAS libsly_core)
