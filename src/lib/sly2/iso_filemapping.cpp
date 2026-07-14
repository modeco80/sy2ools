#include <libsly/sly2/iso_filemapping.hpp>
#include <libsly/sly2/release.hpp>
#include "libsly/sly2/archive_filesystem.hpp"

namespace sly::sly2 {

	// clang-format off

// This declares the name mapping table. This maps both catalog files (files in the ELF's internal catalog)
// and files accessed by bolted catalog entries to "sane" filenames. Once this is completely filled out,
// in theory a "rewal" too may be possible to make which re-inserts WAL FK$ entries.
BEGIN_NAME_MAPPING_TABLE(NtscU, Release)
	// ELF Catalog names
	NAME_BY_FID(0, FileKind::PssMovie, "unk00"),
	NAME_BY_FID(1, FileKind::PssMovie, "unk01"),
	NAME_BY_FID(2, FileKind::PssMovie, "unk02"),
	NAME_BY_FID(3, FileKind::PssMovie, "unk03"),
	NAME_BY_FID(4, FileKind::PssMovie, "unk04"),
	NAME_BY_FID(5, FileKind::PssMovie, "unk05"),
	NAME_BY_FID(6, FileKind::PssMovie, "unk06"),
	NAME_BY_FID(7, FileKind::PssMovie, "unk07"),
	NAME_BY_FID(8, FileKind::PssMovie, "unk08"),
	NAME_BY_FID(9, FileKind::PssMovie, "unk09"),
	NAME_BY_FID(10, FileKind::PssMovie, "unk0a"),
	NAME_BY_FID(11, FileKind::PssMovie, "unk0b"),
	NAME_BY_FID(12, FileKind::PssMovie, "unk0c"),
	NAME_BY_FID(13, FileKind::PssMovie, "unk0d"),
	NAME_BY_FID(14, FileKind::PssMovie, "unk0e"),
	NAME_BY_FID(15, FileKind::PssMovie, "unk0f"),
	NAME_BY_FID(16, FileKind::PssMovie, "unk10"),
	NAME_BY_FID(17, FileKind::PssMovie, "unk11"),
	NAME_BY_FID(18, FileKind::PssMovie, "unk12"),
	NAME_BY_FID(19, FileKind::PssMovie, "unk13"),
	NAME_BY_FID(20, FileKind::PssMovie, "unk14"),
	NAME_BY_FID(21, FileKind::PssMovie, "unk15"),
	NAME_BY_FID(22, FileKind::PssMovie, "unk16"),
	NAME_BY_FID(23, FileKind::PssMovie, "unk17"),
	NAME_BY_FID(24, FileKind::PssMovie, "splash"),
	NAME_BY_FID(25, FileKind::BrxWorld, "jb_intro"),
	NAME_BY_FID(26, FileKind::IconMemcard, "memcard_icon"),
	NAME_BY_FID(28, FileKind::PssMovie, "unk18"),
	NAME_BY_FID(29, FileKind::PssMovie, "unk19"),
	// CD Sector names.

	NAME_BY_LBA(0x00053460, 11696128, FileKind::VpkMusic, "jb_intro"),
	NAME_BY_LBA(0x00052500, 1206504, FileKind::BnkAudio, "jb_intro"),

	// Effects
	NAME_BY_LBA(0x000527c0, 6588416, FileKind::VpkEffects, "jb_intro_effects_1"),

	// voice lines
	NAME_BY_LBA(0x00125a22, 53344, FileKind::VagEnglish, "mission_abandon_sly_murray_bnt_radio_1"),
	NAME_BY_LBA(0x00125a3d, 49920, FileKind::VagEnglish, "mission_abandon_bentley_bnt_1"),

	// Brx worlds
	NAME_BY_LBA(0x00054b20, 1949759, FileKind::BrxWorld, "dvd_menu"),
	NAME_BY_LBA(0x000559a0, 10938886, FileKind::BrxWorld, "f_nightclub_exterior"),
	NAME_BY_LBA(0x0005b640, 6651154, FileKind::BrxWorld, "f_nightclub_cellar"),
	NAME_BY_LBA(0x0005e200, 7645080, FileKind::BrxWorld, "f_nightclub_disco"),
	NAME_BY_LBA(0x00065240, 5227776, FileKind::BrxWorld, "f_nightclub_heist"),
	NAME_BY_LBA(0x00061980, 5751263, FileKind::BrxWorld, "f_nightclub_moulinrouge"),
	NAME_BY_LBA(0x00063240, 4908636, FileKind::BrxWorld, "f_nightclub_waterpump"),
	NAME_BY_LBA(0x000678a0, 11689480, FileKind::BrxWorld, "i_palace_ext"),
	NAME_BY_LBA(0x0006bd40, 7786626, FileKind::BrxWorld, "i_palace_int"),
	NAME_BY_LBA(0x0006dfe0, 6104199, FileKind::BrxWorld, "i_palace_basement"),
	NAME_BY_LBA(0x00070c00, 6354785, FileKind::BrxWorld, "i_palace_ballroom"),
	NAME_BY_LBA(0x00075120, 12361630, FileKind::BrxWorld, "i_temple_ext"),
	NAME_BY_LBA(0x0007b7e0, 7747866, FileKind::BrxWorld, "i_temple_int"),
	NAME_BY_LBA(0x0007e7e0, 11383853, FileKind::BrxWorld, "p_prison_ext"),
	NAME_BY_LBA(0x00083160, 7119379, FileKind::BrxWorld, "p_prison_int"),
	NAME_BY_LBA(0x000862a0, 3737282, FileKind::BrxWorld, "p_prison_vault"),
	NAME_BY_LBA(0x00088400, 11111409, FileKind::BrxWorld, "p_castle_ext"),
	NAME_BY_LBA(0x0008c880, 4208012, FileKind::BrxWorld, "p_castle_waterrails"),
	NAME_BY_LBA(0x00092960, 5363039, FileKind::BrxWorld, "p_castle_guardbreak"),
	NAME_BY_LBA(0x00095720, 4588804, FileKind::BrxWorld, "p_castle_wolftomb"),
	NAME_BY_LBA(0x0008f3c0, 7234451, FileKind::BrxWorld, "p_castle_heist"),
	NAME_BY_LBA(0x00098360, 4456544, FileKind::BrxWorld, "p_castle_ewoktrainer"),
	NAME_BY_LBA(0x0009dac0, 4500407, FileKind::BrxWorld, "p_castle_ewoktomb"),
	NAME_BY_LBA(0x0009af40, 4273600, FileKind::BrxWorld, "p_castle_ewokhall"),
	NAME_BY_LBA(0x000a06c0, 4155295, FileKind::BrxWorld, "p_castle_ewokwater"),
	NAME_BY_LBA(0x000a3200, 9734627, FileKind::BrxWorld, "c_train_ext"),
	NAME_BY_LBA(0x000a70e0, 5117219, FileKind::BrxWorld, "c_train_cabins"),
	NAME_BY_LBA(0x000a9d80, 6442506, FileKind::BrxWorld, "c_train_a"),
	NAME_BY_LBA(0x000acae0, 6689122, FileKind::BrxWorld, "c_train_b"),
	NAME_BY_LBA(0x000af240, 6260457, FileKind::BrxWorld, "c_train_c"),
	NAME_BY_LBA(0x000b18c0, 10965082, FileKind::BrxWorld, "c_sawmill_ext"),
	NAME_BY_LBA(0x000b6260, 4755428, FileKind::BrxWorld, "c_sawmill_mulch"),
	NAME_BY_LBA(0x000b92a0, 5074218, FileKind::BrxWorld, "c_sawmill_burn"),
	NAME_BY_LBA(0x000bbf60, 4974075, FileKind::BrxWorld, "c_sawmill_lighthouse"),
	NAME_BY_LBA(0x000bec00, 3715670, FileKind::BrxWorld, "c_sawmill_bearcave"),
	NAME_BY_LBA(0x000c1600, 4752435, FileKind::BrxWorld, "c_sawmill_boss"),
	NAME_BY_LBA(0x000c4860, 9793104, FileKind::BrxWorld, "a_blimp_ext"),
	NAME_BY_LBA(0x000c8300, 6186356, FileKind::BrxWorld, "a_blimp_int"),
	NAME_BY_LBA(0x000cdb00, 4904282, FileKind::BrxWorld, "a_blimp_engine_room_murray"),
	NAME_BY_LBA(0x000cb100, 4317986, FileKind::BrxWorld, "a_blimp_engine_room_bentley"),
	NAME_BY_LBA(0x000d06a0, 4271749, FileKind::BrxWorld, "a_blimp_engine_room_jt"),
	NAME_BY_LBA(0x000d30a0, 9209356, FileKind::BrxWorld, "a_blimp_boss_final"),
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
