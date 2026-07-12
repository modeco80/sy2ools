#include <cstring>
#include <libsly/core/sha256.hpp>
#include <libsly/sly2/iso_filemapping.hpp>
#include <libsly/sly2/iso_filesystem.hpp>
#include <libsly/sly2/release.hpp>
#include <mco/io/file_stream.hpp>

namespace sly::sly2 {

	// in iso_filemapping.cpp. Just private because we don't really need it
	const ReleaseCdDataTable* getCdDataForRelease(Release release);

	constexpr u64 lbaToOffset(u32 lba) {
		return static_cast<u64>(lba) * 0x800;
	}

	class IsoFileStream : public mco::Stream {
		u64 virtualSeekPtr;
		u32 lbaStart;
		u32 fileSize;

		mco::FileStream& isoStream;

	   public:
		IsoFileStream(IsoFileSystem& fs, u32 lbaStart, u32 fileSize)
			: isoStream(fs.isoFile), lbaStart(lbaStart), fileSize(fileSize) {
			seek(0, Begin);
		}

		bool isRandomAccess() const noexcept override {
			return true;
		}

		u64 read(void* buffer, u64 length) override {
			seek(virtualSeekPtr, mco::Stream::Begin);

			// EOF handling
			if(virtualSeekPtr >= fileSize) {
				return 0;
			}

			std::size_t readCount = length;
			if(virtualSeekPtr + length > fileSize)
				readCount = fileSize - virtualSeekPtr;

			auto nRead = isoStream.read(buffer, readCount);
			virtualSeekPtr += nRead;
			return nRead;
		}

		i64 tell() override {
			return virtualSeekPtr;
		}

		i64 seek(i64 offset, Whence whence) override {
			u64 target = 0;

			switch(whence) {
				case mco::Stream::Begin:
					target = offset;
					break;
				case mco::Stream::Current:
					target = virtualSeekPtr + offset;
					break;
				case mco::Stream::End:
					if(offset > 0)
						return -1;
					target = fileSize + offset;
					break;
			}

			if(target > fileSize) {
				// too far
				return -1;
			}

			isoStream.seek(lbaToOffset(lbaStart) + target, mco::Stream::Begin);
			virtualSeekPtr = target;
			return virtualSeekPtr;
		}

		u64 getSize() override {
			return fileSize;
		}
	};

	void IsoFileSystem::guessRelease() {
		// Do sha256 digest on the first 1MB of the ISO file.
		isoFile.seek(0, mco::Stream::Begin);
		auto digest = sly::core::sha256Digest(isoFile, 0x100000);
		isoFile.seek(0, mco::Stream::Begin);

		bool matchedRelease = false;

		forEachGameRelease([&](Release release) {
			auto releaseDigest = *getDigestForRelease(release);
			if(digest == releaseDigest) {
				matchedRelease = true;
				this->release = release;
				return false;
			}

			return true;
		});

		// Give up.
		if(!matchedRelease)
			return;

		// Init
		releaseData = getCdDataForRelease(release);
		initCdCatalog();
	}

	void IsoFileSystem::initCdCatalog() {
		// Can't do anything without the release data.
		if(releaseData == nullptr)
			return;

		cdCatalog = new CdCatalogEntry[releaseData->countElfCdCatalog];
		const auto nCatalogBytes = releaseData->countElfCdCatalog * sizeof(CdCatalogEntry);

		isoFile.seek(lbaToOffset(releaseData->lbaElfBase) + releaseData->offsetElfCdCatalog, mco::Stream::Begin);
		if(isoFile.read(&cdCatalog[0], nCatalogBytes) != nCatalogBytes) {
			throw std::runtime_error("Short read of ELF CD catalog. This should NEVER happen!");
		}
	}

	IsoFileSystem::IsoFileSystem(mco::FileStream&& fs)
		: isoFile(std::move(fs)) {
		// Reset to very invalid state.
		release = Release::Invalid;
		cdCatalog = nullptr;
		releaseData = nullptr;
		guessRelease();
	}

	IsoFileSystem::~IsoFileSystem() {
		if(cdCatalog) {
			delete[] cdCatalog;
		}
	}

	Release IsoFileSystem::getRelease() const {
		return release;
	}

	mco::Stream* IsoFileSystem::openFileByFid(IsoFileId fid) {
		if(fid > releaseData->countElfCdCatalog)
			return nullptr;

		const auto& catalogEntry = cdCatalog[fid];
		return openFileByCdSector(catalogEntry.getLba() + releaseData->lbaCdDataBase, catalogEntry.getSize());
	}

	mco::Stream* IsoFileSystem::openFileByCdSector(u32 lba, u32 cb) {
		mFileStreams.push_back(std::make_unique<IsoFileStream>(*this, lba, cb));
		return mFileStreams.back().get();
	}

	ArchiveKind IsoFileSystem::getKind() const {
		return IsoFileSystem::Kind;
	}

	void IsoFileSystem::enumFiles(bool (*pcb)(const char* pszFileName, u32 size, void* user), void* user) const {
		for(auto i = 0; i < releaseData->nameMapTableCount; ++i) {
			const auto& mapent = releaseData->nameMapTable[i];
			u32 size;

			if(mapent.kind == NameMappingTableEntry::MappingKind::Fid) {
				// need to reach the cd table to get the size for FID entries.
				size = cdCatalog[mapent.fid].getSize();
			} else if(mapent.kind == NameMappingTableEntry::MappingKind::CdSector) {
				size = mapent.size;
			}

			if(!pcb(mapent.pszFileName, size, user))
				return;
		}
	}

	mco::Stream* IsoFileSystem::openFile(const char* pszName) {
		// Look through the name mapping table, for a matching file name.
		for(auto i = 0; i < releaseData->nameMapTableCount; ++i) {
			const auto& mapent = releaseData->nameMapTable[i];
			if(!std::strcmp(mapent.pszFileName, pszName)) {
				// If the name matches, depending on the kind of file, open it.
				if(mapent.kind == NameMappingTableEntry::MappingKind::Fid)
					return openFileByFid(mapent.fid);
				else if(mapent.kind == NameMappingTableEntry::MappingKind::CdSector)
					return openFileByCdSector(mapent.lbaStart, mapent.size);
			}
		}

		return nullptr;
	}

	void IsoFileSystem::closeFile(mco::Stream* stream) {
		// TODO
		printf("Warning IsoFileSystem::closeFile() not yet implemented, leaking this file for now..\n");
	}

} // namespace sly::sly2
