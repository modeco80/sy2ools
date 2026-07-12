#pragma once
#include <libsly/sly2/archive_filesystem.hpp>
#include <libsly/sly2/iso_filesystem_types.hpp>
#include <libsly/sly2/release.hpp>
#include <mco/io/file_stream.hpp>
#include <mco/io/stream.hpp>
#include <vector>

namespace sly::sly2 {

	using IsoFileId = u32;
	struct ReleaseCdDataTable;

	/// This class manages opening files out of final builds of Sly 2.
	/// Final builds of Sly 2 hard-bolt their files on the disc,
	/// subverting the ISO 9660 & UDF filesystems entirely.
	///
	/// This was seemingly only done for obfuscation purposes,
	/// or I guess to thwart off "dumb" CD/DVD dumping utilities that would only dump ISO filesystem/sessions.
	class IsoFileSystem : public IArchiveFileSystem {
		friend class IsoFileStream;
		mco::FileStream isoFile;

		Release release;
		const ReleaseCdDataTable* releaseData;
		CdCatalogEntry* cdCatalog;

		std::vector<std::unique_ptr<mco::Stream>> mFileStreams;

		void guessRelease();
		void initCdCatalog();

	   public:
		explicit IsoFileSystem(mco::FileStream&& isoFile);
		IsoFileSystem(const IsoFileSystem&) = delete;
		IsoFileSystem(IsoFileSystem&&) = delete; // for now?
		~IsoFileSystem();

		Release getRelease() const;

		mco::Stream* openFileByFid(IsoFileId fid);
		mco::Stream* openFileByCdSector(u32 lba, u32 cb);

		// IArchiveFileSystem
		constexpr static ArchiveKind Kind = 0x0;

		ArchiveKind getKind() const override;

		void enumFiles(bool (*pcb)(const char* pszFileName, u32 size, void* user), void* user) const override;

		/// Opens a file by name. Name mapping table must be complete.
		mco::Stream* openFile(const char* pszName) override;

		void closeFile(mco::Stream* stream) override;
	};

} // namespace sly::sly2
