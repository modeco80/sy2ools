#include <cstdio>
#include <mco/io/stream_utils.hpp>
#include <mco/utils.hpp>
#include <mco/io/file_stream.hpp>
#include <libsly/core/buffer_streams.hpp>

#include "cmd.hpp"
#include "utils.hpp"

namespace slyarc {

	namespace {
		void commandExtractAllHelp() {
			std::printf("Extracts all files in the archive\n");
		}

		int commandExtractAll(int argc, char** argv) {
			if(argc != 1) {
				std::printf("Error: no archive given\n");
				return 1;
			}

			try {
				auto fs = slyarc::createArchiveFs(std::filesystem::path(argv[0]));
				fs->enumFilesLambda([&](const char* pszFileName, u32 fileSize) {
					auto* file = fs->openFile(pszFileName);
					if(!file) {
						std::printf("Couldn't open file %s in archive %s??\n", pszFileName, argv[0]);
						return false;
					}

					auto fileOut = mco::FileStream::open(pszFileName, mco::FileStream::Create|mco::FileStream::ReadWrite);
					auto bufferWrite = sly::core::BufferedWriteStream(fileOut, 0x2000);
					mco::teeStreams(*file, bufferWrite, fileSize);
					return true;
				});
			} catch(std::exception& ex) {
				std::printf("Error extracting all from archive %s: %s\n", argv[0], ex.what());
				return 1;
			}

			return 0;
		}
	} // namespace

	static Command cmdExtractAll('x', &commandExtractAllHelp, &commandExtractAll);
} // namespace slyarc
