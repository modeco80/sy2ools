#pragma once
#include <mco/io/stream.hpp>
#include <vector>

namespace sly::sly2 {

	using ArchiveKind = u8;
	constexpr ArchiveKind DefaultArchiveKind = 0xff;

	struct FileInformation {
		std::string fileName;
		u32 fileSize;
	};

	/// Abstraction over archive files..
	class IArchiveFileSystem {
	   public:
		virtual ~IArchiveFileSystem() = default;

		/// Returns the kind of the archive. Any archive types which expose special behaviour
		/// can override this so that clients can tell what type of IArchiveFileSystem they got
		/// without RTTI. If no special behaviour is provided than you can just use this default implementation.
		virtual ArchiveKind getKind() const {
			return DefaultArchiveKind;
		}

		/// Enumerates all files in the archive.
		virtual void enumFiles(bool (*pcb)(const char* pszFileName, u32 size, void* user), void* user) const = 0;

		template <class F>
		void enumFilesLambda(F&& fun) const {
			enumFiles([](const char* pszFileName, u32 size, void* user) -> bool {
				return (*reinterpret_cast<F*>(user))(pszFileName, size);
			},
					  &fun);
		}

		std::vector<FileInformation> getFiles() const;

		virtual mco::Stream* openFile(const char* pszName) = 0;

		virtual void closeFile(mco::Stream* file) = 0;
	};

} // namespace sly::sly2
