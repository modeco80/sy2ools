#pragma once
#include <libsly/sly2/file_location.hpp>
#include <libsly/sly2/release.hpp>
#include <mco/io/stream.hpp>
#include <vector>

namespace sly::sly2 {

	using ArchiveKind = u8;
	constexpr ArchiveKind DefaultArchiveKind = 0xff;

	// TODO: Fill this out
	/// File kind. This directly maps to FK$ key codes.
	enum class FileKind : u8 {
		BrxWorld = 'Z',
		IconMemcard = 'I',

		// Audio
		BnkAudio = 'B',
		VpkMusic = 'M',
		VpkEffects = 'E',

		VagEnglish = 'e',

		PssMovie = 'P',
	};

	struct FileInformation {
		FileKind kind;
		std::string fileName;
		u32 fileSize;
	};

	/// An archive filesystem abstraction.
	class IArchiveFileSystem {
	   public:
		virtual ~IArchiveFileSystem() = default;

		/// Returns the kind of the archive. Any archive types which expose special behaviour
		/// can override this so that clients can tell what type of IArchiveFileSystem they got
		/// without RTTI. If no special behaviour is provided than you can just use this default implementation.
		virtual ArchiveKind getKind() const {
			return DefaultArchiveKind;
		}

		virtual Release getRelease() const = 0;

		virtual void enumFilesImpl(bool (*pcb)(const char* pszFileName, u32 size, void* user), void* user) const = 0;

		/// Enumerates all files in the archive.
		template <class F>
		void enumFiles(F&& fun) const {
			enumFilesImpl([](const char* pszFileName, u32 size, void* user) -> bool {
				return (*reinterpret_cast<F*>(user))(pszFileName, size);
			},
						  &fun);
		}

		/// Makes a "sane" file name from a FK$ lookup string.
		void makeSaneFilename(char* pszOut, const char* pszFkLookup);

		std::vector<FileInformation> getFiles() const;

		/// Opens a file by location.
		virtual mco::Stream* openFile(const FileLocation& loc) = 0;

		/// Closes a file previously opened by [openFile()].
		virtual void closeFile(mco::Stream* file) = 0;
	};

} // namespace sly::sly2
