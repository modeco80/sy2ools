#include <libsly/sly2/archive_filesystem.hpp>

#include "fk_utils.hpp"

namespace sly::sly2 {

	void IArchiveFileSystem::makeSaneFilename(char* pszOut, const char* pszFkLookup) {
		if(!validFkLookup(pszFkLookup)) {
			// no thank you.
			std::strcpy(pszOut, pszFkLookup);
			return;
		}

		auto fkType = getFkLookupType(pszFkLookup);
		auto searchName = getFkSearchName(pszFkLookup);

		using enum FileKind;
		char ext[8] {};

		switch(fkType) {
			case PssMovie:
				std::strcpy(ext, "pss");
				break;
			case BrxWorld:
				std::strcpy(ext, "brx");
				break;
			case IconMemcard:
				std::strcpy(ext, "ico");
				break;
			default:
				sprintf(&ext[0], "FK%c", fkType);
				return;
		}

		std::snprintf(pszOut, 0x40, "%s.%s", searchName, ext);
	}

	std::vector<FileInformation> IArchiveFileSystem::getFiles() const {
		std::vector<FileInformation> files;

		enumFiles([&](const char* pszFileName, u32 size) {
			files.push_back(FileInformation {
			.fileName = std::string(pszFileName),
			.fileSize = size });
			return true; // Keep iterating
		});

		return files;
	}

} // namespace sly::sly2
