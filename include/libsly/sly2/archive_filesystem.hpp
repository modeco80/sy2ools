#pragma once
#include <mco/io/stream.hpp>

namespace sly::sly2 {

	using ArchiveKind = u8;
	constexpr ArchiveKind DefaultArchiveKind = 0xff;

	class FileInformation {
	public:
		std::string fileName;
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

		// TODO: API to enumerate through known files.

		virtual mco::Stream* openFile(const char* pszName) = 0;

		virtual void closeFile(mco::Stream* file) = 0;
	};

} // namespace sly::sly2
