#pragma once
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

		const char* pszFileName;
	};

	struct ReleaseCdMappingTable {
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
#define NAME_BY_FID(_fid, name) \
	{ .kind = NameMappingTableEntry::MappingKind::Fid, .fid = _fid, .pszFileName = name }

#define NAME_BY_LBA(_lba, _cb, name) \
	{ .kind = NameMappingTableEntry::MappingKind::CdSector, .lbaStart = _lba, .size = _cb, .pszFileName = name }

#define END_NAME_MAPPING_TABLE() \
	}

#define MAKE_RELEASE(region, version, _lbaElfBase, _offsetElfCdCatalog, _countElfCdCatalog, _lbaCdDataBase) \
	constexpr static ReleaseCdMappingTable sCatalogEntry_##region##_##version = {                           \
		.lbaElfBase = _lbaElfBase,                                                                          \
		.offsetElfCdCatalog = _offsetElfCdCatalog,                                                          \
		.countElfCdCatalog = _countElfCdCatalog,                                                            \
		.lbaCdDataBase = _lbaCdDataBase,                                                                    \
		.nameMapTableCount = sizeof(sMappingTable_##region##_##version) / sizeof(NameMappingTableEntry),    \
		.nameMapTable = &sMappingTable_##region##_##version[0]                                              \
	}

} // namespace sly::sly2
