#include <libsly/sly2/archive_filesystem.hpp>

namespace sly::sly2 {

	std::vector<FileInformation> IArchiveFileSystem::getFiles() const {
		std::vector<FileInformation> files;

		enumFilesLambda([&](const char* pszFileName, u32 size) {
			files.push_back(FileInformation {
			.fileName = std::string(pszFileName),
			.fileSize = size });
			return true; // Keep iterating
		});

		return files;
	}

} // namespace sly::sly2
