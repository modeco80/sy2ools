#include <cstdio>
#include <libsly/core/buffer_streams.hpp>
#include <mco/io/file_stream.hpp>
#include <mco/io/stream_utils.hpp>
#include <mco/utils.hpp>

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

					char saneFileName[0x40] {};
					fs->makeSaneFilename(&saneFileName[0], pszFileName);

					auto fileOut = mco::FileStream::open(&saneFileName[0], mco::FileStream::Create | mco::FileStream::ReadWrite);
					auto bufferWrite = sly::core::BufferedWriteStream(fileOut, 0x2000);
					mco::teeStreams(*file, bufferWrite, fileSize);

					printf("extracted %s (%s) to %s\n", pszFileName, mco::makeHumanReadableByteSize(fileSize).c_str(), &saneFileName[0]);
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
