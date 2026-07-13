#pragma once
#include <libsly/sly2/archive_filesystem.hpp>
#include <mco/base_types.hpp>

namespace sly::sly2 {

	struct NameMappingTableEntry {
		enum class MappingKind : u32 {
			Fid = 0,
			CdSector = 1
		};

		MappingKind kind;
		union {
			// MappingKind::Fid
			struct {
				u32 fid;
			};

			// MappingKind::CdSector
			struct {
				u32 lbaStart;
				u32 size;
			};
		};

		FileKind fk;
		const char* pszFileName;
	};

	struct ReleaseCdDataTable {
		u32 lbaElfBase;
		u32 offsetElfCdCatalog;
		u32 countElfCdCatalog;

		u32 lbaCdDataBase;

		// Name mapping table.
		u32 nameMapTableCount;
		const NameMappingTableEntry* nameMapTable;
	};

#define BEGIN_NAME_MAPPING_TABLE(region, version) \
	constexpr static NameMappingTableEntry sMappingTable_##region##_##version[] = {
/// Add a by-FID mapping
#define NAME_BY_FID(_fid, _fk, name) \
	{ .kind = NameMappingTableEntry::MappingKind::Fid, .fid = _fid, .fk = _fk, .pszFileName = name }

#define NAME_BY_LBA(_lba, _cb, _fk, name) \
	{ .kind = NameMappingTableEntry::MappingKind::CdSector, .lbaStart = _lba, .size = _cb, .fk = _fk, .pszFileName = name }

#define END_NAME_MAPPING_TABLE() \
	}

#define MAKE_RELEASE(region, version, _lbaElfBase, _offsetElfCdCatalog, _countElfCdCatalog, _lbaCdDataBase) \
	constexpr static ReleaseCdDataTable sCatalogEntry_##region##_##version = {                              \
		.lbaElfBase = _lbaElfBase,                                                                          \
		.offsetElfCdCatalog = _offsetElfCdCatalog,                                                          \
		.countElfCdCatalog = _countElfCdCatalog,                                                            \
		.lbaCdDataBase = _lbaCdDataBase,                                                                    \
		.nameMapTableCount = sizeof(sMappingTable_##region##_##version) / sizeof(NameMappingTableEntry),    \
		.nameMapTable = &sMappingTable_##region##_##version[0]                                              \
	}

} // namespace sly::sly2
