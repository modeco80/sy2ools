#include <cstdio>
#include <mco/utils.hpp>

#include "cmd.hpp"
#include "utils.hpp"

namespace slyarc {

	namespace {
		void commandListHelp() {
			std::printf("Lists all files in the archive\n");
		}

		int commandList(int argc, char** argv) {
			if(argc != 1) {
				std::printf("Error: no archive given\n");
				return 1;
			}

			try {
				auto fs = slyarc::createArchiveFs(std::filesystem::path(argv[0]));
				fs->enumFiles([&](const char* pszFileName, u32 size) {
					char saneName[0x40]{};
					fs->makeSaneFilename(&saneName[0], pszFileName);

					printf("%s (%s) %s\n", pszFileName, &saneName[0], mco::makeHumanReadableByteSize(size).c_str());
					return true;
				});
			} catch(std::exception& ex) {
				std::printf("Error listing %s: %s\n", argv[0], ex.what());
				return 1;
			}

			return 0;
		}
	} // namespace

	static Command cmdList('l', &commandListHelp, &commandList);
} // namespace slyarc
