#pragma once
#include <libsly/sly2/release.hpp>
#include <mco/io/file_stream.hpp>
#include <mco/io/stream.hpp>
#include <vector>

namespace sly::sly2 {

	using FileId = u32;
	struct ReleaseCdMappingTable;
	struct CdCatalogEntry;

	/// This class manages opening files out of the Sly 2 disc. Final builds of Sly 2 hard-bolt their
	/// files on the disc, subverting the ISO 9660 & UDF filesystems entirely. This was seemingly only
	/// done for obfuscation purposes, I guess to thwart off "dumb" CD/DVD dumping utilities that would
	/// only dump ISO filesystem/sessions.
	class IsoFileSystem {
		mco::FileStream isoFile;

		Release release;
		const ReleaseCdMappingTable* releaseCatalog;
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

		mco::Stream* openFileByFid(FileId fid);
		mco::Stream* openFileByCdSector(u32 lba, u32 cb);

		/// Opens a file by name. Name mapping table must be complete.
		mco::Stream* openFileByName(const char* pszName);

		void closeFile(mco::Stream* stream);
	};

} // namespace sly::sly2
