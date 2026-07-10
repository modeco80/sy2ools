cmake_minimum_required(VERSION 3.10..4.69)

# Prohibit in-source tree builds.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "In-source builds are strictly prohibited.")
endif()

project(sy2tools
        VERSION 0.1.0
        LANGUAGES C CXX
        )

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(sy2Policies)
include(sy2ProjectFuncs)

option(LIBSLY_USE_OPENSSL "Use OpenSSL library for crypto." ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Third party dependencies
add_subdirectory(third_party/mcolib)

# libsly libraries
add_subdirectory(src/lib/core)
add_subdirectory(src/lib/sly2)

# command line tools
add_subdirectory(src/tools/cli)
