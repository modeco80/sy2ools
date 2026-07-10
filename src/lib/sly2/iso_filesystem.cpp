#include <libsly/core/sha256.hpp>
#include <libsly/sly2/iso_filemapping.hpp>
#include <libsly/sly2/iso_filesystem.hpp>
#include <libsly/sly2/release.hpp>

#include <mco/io/file_stream.hpp>
#include <cstring>

namespace sly::sly2 {

	// in iso_filemapping.cpp. Just private because we don't really need it
	const ReleaseCdMappingTable* getCatalogEntryForRelease(Release release);

	/// CD catalog entry
	struct CdCatalogEntry {
		u32 lba;
		u32 size;
		u32 unk;
		u8 unk2[40]; // TODO this could be interesting data
		u32 lbaXorKey;
		u32 unkXorKey;
		u32 sizeXorKey;

		u32 getSize() const {
			return size ^ sizeXorKey;
		}

		u32 getLba() const {
			return (lba ^ lbaXorKey);
		}
	};

	constexpr u64 lbaToOffset(u32 lba) {
		return static_cast<u64>(lba) * 0x800;
	}

	void IsoFileSystem::guessRelease() {
		// Do sha256 digest on the ISO file.
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
		releaseCatalog = getCatalogEntryForRelease(release);
		initCdCatalog();
	}

	void IsoFileSystem::initCdCatalog() {
		// Can't do anything without the release catalog data.
		if(releaseCatalog == nullptr)
			return;

		cdCatalog = new CdCatalogEntry[releaseCatalog->countElfCdCatalog];
		const auto nCatalogBytes = releaseCatalog->countElfCdCatalog * sizeof(CdCatalogEntry);

		isoFile.seek(lbaToOffset(releaseCatalog->lbaElfBase) + releaseCatalog->offsetElfCdCatalog, mco::Stream::Begin);
		if(isoFile.read(&cdCatalog[0], nCatalogBytes) != nCatalogBytes) {
			throw std::runtime_error("Short read of CD root catalog. This should NEVER happen!");
		}
	}

	IsoFileSystem::IsoFileSystem(mco::FileStream&& fs)
		: isoFile(std::move(fs)) {
		// Reset to very invalid state.
		release = Release::Invalid;
		cdCatalog = nullptr;
		releaseCatalog = nullptr;
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

	mco::Stream* IsoFileSystem::openFileByFid(FileId fid) {
		return nullptr;
	}

	mco::Stream* IsoFileSystem::openFileByCdSector(u32 lba, u32 cb) {
		return nullptr;
	}

	/// Opens a file by name. Name mapping table must be complete.
	mco::Stream* IsoFileSystem::openFileByName(const char* pszName) {
		for(auto i = 0; i < releaseCatalog->nameMapTableCount; ++i) {
			const auto& mapent = releaseCatalog->nameMapTable[i];
			if(!std::strcmp(mapent.pszFileName, pszName)) {
				if(mapent.kind == NameMappingTableEntry::MappingKind::Fid)
					return openFileByFid(mapent.fid);
				else if(mapent.kind == NameMappingTableEntry::MappingKind::CdSector)
					return openFileByCdSector(mapent.lbaStart, mapent.size);
			}
		}

		return nullptr;
	}

	void IsoFileSystem::closeFile(mco::Stream* stream) {
	}

} // namespace sly::sly2
