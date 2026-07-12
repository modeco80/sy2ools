#include <libsly/sly2/iso_filemapping.hpp>
#include <libsly/sly2/release.hpp>

namespace sly::sly2 {

	// clang-format off

// This declares the name mapping table. This maps both catalog files (files in the ELF's internal catalog)
// and files accessed by bolted catalog entries to "sane" filenames. Once this is completely filled out,
// in theory a "rewal" too may be possible to make which re-inserts WAL FK$ entries.
BEGIN_NAME_MAPPING_TABLE(NtscU, Release)
	// Catalog names
	// TODO: bunch of pssmovies which are in this table.
	NAME_BY_FID(24, FileKind::PssMovie, "splash.pss"),
	NAME_BY_FID(25, FileKind::BrxMap, "intro.brx"),
	NAME_BY_FID(26, FileKind::IconMemcard, "memcard_icon.ico"),
	// CD Sector names.

	NAME_BY_LBA(0x00054b20, 1949759, FileKind::BrxMap, "unk02.brx"),
	NAME_BY_LBA(0x000559a0, 10938886, FileKind::BrxMap, "unk03.brx"),
	NAME_BY_LBA(0x0005b640, 6651154, FileKind::BrxMap, "unk04.brx"),
	NAME_BY_LBA(0x0005e200, 7645080, FileKind::BrxMap, "unk05.brx"),
	NAME_BY_LBA(0x00065240, 5227776, FileKind::BrxMap, "unk06.brx"),
	NAME_BY_LBA(0x00061980, 5751263, FileKind::BrxMap, "unk07.brx"),
	NAME_BY_LBA(0x00063240, 4908636, FileKind::BrxMap, "unk08.brx"),
	NAME_BY_LBA(0x000678a0, 11689480, FileKind::BrxMap, "unk09.brx"),
	NAME_BY_LBA(0x0006bd40, 7786626, FileKind::BrxMap, "unk0a.brx"),
	NAME_BY_LBA(0x0006dfe0, 6104199, FileKind::BrxMap, "unk0b.brx"),
	NAME_BY_LBA(0x00070c00, 6354785, FileKind::BrxMap, "unk0c.brx"),
	NAME_BY_LBA(0x00075120, 12361630, FileKind::BrxMap, "unk0d.brx"),
	NAME_BY_LBA(0x0007b7e0, 7747866, FileKind::BrxMap, "unk0e.brx"),
	NAME_BY_LBA(0x0007e7e0, 11383853, FileKind::BrxMap, "unk0f.brx"),
	NAME_BY_LBA(0x00083160, 7119379, FileKind::BrxMap, "unk10.brx"),
	NAME_BY_LBA(0x000862a0, 3737282, FileKind::BrxMap, "unk11.brx"),
	NAME_BY_LBA(0x00088400, 11111409, FileKind::BrxMap, "unk12.brx"),
	NAME_BY_LBA(0x0008c880, 4208012, FileKind::BrxMap, "unk13.brx"),
	NAME_BY_LBA(0x00092960, 5363039, FileKind::BrxMap, "unk14.brx"),
	NAME_BY_LBA(0x00095720, 4588804, FileKind::BrxMap, "unk15.brx"),
	NAME_BY_LBA(0x0008f3c0, 7234451, FileKind::BrxMap, "unk16.brx"),
	NAME_BY_LBA(0x00098360, 4456544, FileKind::BrxMap, "unk17.brx"),
	NAME_BY_LBA(0x0009dac0, 4500407, FileKind::BrxMap, "unk18.brx"),
	NAME_BY_LBA(0x0009af40, 4273600, FileKind::BrxMap, "unk19.brx"),
	NAME_BY_LBA(0x000a06c0, 4155295, FileKind::BrxMap, "unk20.brx"),
	NAME_BY_LBA(0x000a3200, 9734627, FileKind::BrxMap, "unk21.brx"),
	NAME_BY_LBA(0x000a70e0, 5117219, FileKind::BrxMap, "unk22.brx"),
	NAME_BY_LBA(0x000a9d80, 6442506, FileKind::BrxMap, "unk23.brx"),
	NAME_BY_LBA(0x000acae0, 6689122, FileKind::BrxMap, "unk24.brx"),
	NAME_BY_LBA(0x000af240, 6260457, FileKind::BrxMap, "unk25.brx"),
	NAME_BY_LBA(0x000b18c0, 10965082, FileKind::BrxMap, "unk26.brx"),
	NAME_BY_LBA(0x000b6260, 4755428, FileKind::BrxMap, "unk27.brx"),
	NAME_BY_LBA(0x000b92a0, 5074218, FileKind::BrxMap, "unk28.brx"),
	NAME_BY_LBA(0x000bbf60, 4974075, FileKind::BrxMap, "unk29.brx"),
	NAME_BY_LBA(0x000bec00, 3715670, FileKind::BrxMap, "unk2a.brx"),
	NAME_BY_LBA(0x000c1600, 4752435, FileKind::BrxMap, "unk2b.brx"),
	NAME_BY_LBA(0x000c4860, 9793104, FileKind::BrxMap, "unk2c.brx"),
	NAME_BY_LBA(0x000c8300, 6186356, FileKind::BrxMap, "unk2d.brx"),
	NAME_BY_LBA(0x000cdb00, 4904282, FileKind::BrxMap, "unk2e.brx"),
	NAME_BY_LBA(0x000cb100, 4317986, FileKind::BrxMap, "unk2f.brx"),
	NAME_BY_LBA(0x000d06a0, 4271749, FileKind::BrxMap, "unk30.brx"),
	NAME_BY_LBA(0x000d30a0, 9209356, FileKind::BrxMap, "unk31.brx"),
END_NAME_MAPPING_TABLE();

MAKE_RELEASE(NtscU, Release, 0x10f, 0x1a32e8, 30, 3232);
	// clang-format on

	const ReleaseCdDataTable* getCdDataForRelease(Release release) {
#define X(region, version, _sha256) \
	case region##_##version:        \
		return &sCatalogEntry_##region##_##version;
		using enum Release;
		switch(release) {
			LIBSLY_SLY2_RELEASES()
			// Invalid or no mapping table created.
			case Invalid:
			default:
				return nullptr;
		}
#undef RELEASE
	}

} // namespace sly::sly2
