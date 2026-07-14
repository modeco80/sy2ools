#include <libsly/sly2/iso_filemapping.hpp>

// clang-format off

// This declares the name mapping table.
// This maps both files in the ELF's internal catalog
// and files accessed by bolted catalog entries to FK$ filenames.
BEGIN_NAME_MAPPING_TABLE(NtscU, Release)
	// ELF Catalog names
	NAME_BY_FID(0, PssMovie, "unk00"),
	NAME_BY_FID(1, PssMovie, "unk01"),
	NAME_BY_FID(2, PssMovie, "unk02"),
	NAME_BY_FID(3, PssMovie, "unk03"),
	NAME_BY_FID(4, PssMovie, "unk04"),
	NAME_BY_FID(5, PssMovie, "unk05"),
	NAME_BY_FID(6, PssMovie, "unk06"),
	NAME_BY_FID(7, PssMovie, "unk07"),
	NAME_BY_FID(8, PssMovie, "unk08"),
	NAME_BY_FID(9, PssMovie, "unk09"),
	NAME_BY_FID(10, PssMovie, "unk0a"),
	NAME_BY_FID(11, PssMovie, "unk0b"),
	NAME_BY_FID(12, PssMovie, "unk0c"),
	NAME_BY_FID(13, PssMovie, "unk0d"),
	NAME_BY_FID(14, PssMovie, "unk0e"),
	NAME_BY_FID(15, PssMovie, "unk0f"),
	NAME_BY_FID(16, PssMovie, "unk10"),
	NAME_BY_FID(17, PssMovie, "unk11"),
	NAME_BY_FID(18, PssMovie, "unk12"),
	NAME_BY_FID(19, PssMovie, "unk13"),
	NAME_BY_FID(20, PssMovie, "unk14"),
	NAME_BY_FID(21, PssMovie, "unk15"),
	NAME_BY_FID(24, PssMovie, "splash"),
	NAME_BY_FID(25, BrxWorld, "jb_intro"),
	NAME_BY_FID(26, IconMemcard, "memcard_icon"),
	NAME_BY_FID(28, PssMovie, "unk18"),
	NAME_BY_FID(29, PssMovie, "unk19"),
	// CD Sector names.

	NAME_BY_LBA(0x00053460, 11696128, VpkMusic, "jb_intro"),
	NAME_BY_LBA(0x00052500, 1206504, BnkAudio, "jb_intro"),

	// Effects
	NAME_BY_LBA(0x000527c0, 6588416, VpkEffects, "jb_intro_effects_1"),

	// voice lines
	NAME_BY_LBA(0x00125a22, 53344, VagEnglish, "mission_abandon_sly_murray_bnt_radio_1"),
	NAME_BY_LBA(0x00125a3d, 49920, VagEnglish, "mission_abandon_bentley_bnt_1"),
	NAME_BY_LBA(0x00125a56, 65920, VagEnglish, "e0_museum_entry_01_bnt_radio_1"),
	NAME_BY_LBA(0x00125a77, 30368, VagEnglish, "e0_museum_entry_02_sly_1"),
	NAME_BY_LBA(0x00125a86, 55568, VagEnglish, "e0_museum_entry_03_bnt_radio_1"),
	NAME_BY_LBA(0x00125aa2, 29760, VagEnglish, "e0_museum_entry_04_sly_1"),
	NAME_BY_LBA(0x00125ab1, 52880, VagEnglish, "e0_museum_entry_05_bnt_radio_1"),
	NAME_BY_LBA(0x00125acb, 147488, VagEnglish, "e0_museum_entry_06_sly_1"),
	NAME_BY_LBA(0x00125b14, 122720, VagEnglish, "e0_museum_entry_07_bnt_radio_1"),
	NAME_BY_LBA(0x00125b50, 49312, VagEnglish, "e0_museum_entry_08_sly_1"),
	NAME_BY_LBA(0x00125b69, 72896, VagEnglish, "e0_hack_computer_01_bnt_radio_1"),
	NAME_BY_LBA(0x00125b8d, 53008, VagEnglish, "e0_hack_computer_02_bnt_1"),
	NAME_BY_LBA(0x00125ba7, 64496, VagEnglish, "e0_hack_computer_03_bnt_1"),
	NAME_BY_LBA(0x00125bc7, 25872, VagEnglish, "e0_hack_computer_04_bnt_1"),
	NAME_BY_LBA(0x00125bd4, 22640, VagEnglish, "e0_hack_computer_05_bnt_1"),
	NAME_BY_LBA(0x00125be0, 37824, VagEnglish, "e0_hack_computer_06_sly_1"),


	// Brx worlds
	NAME_BY_LBA(0x00054b20, 1949759, BrxWorld, "dvd_menu"),
	NAME_BY_LBA(0x000559a0, 10938886, BrxWorld, "f_nightclub_exterior"),
	NAME_BY_LBA(0x0005b640, 6651154, BrxWorld, "f_nightclub_cellar"),
	NAME_BY_LBA(0x0005e200, 7645080, BrxWorld, "f_nightclub_disco"),
	NAME_BY_LBA(0x00065240, 5227776, BrxWorld, "f_nightclub_heist"),
	NAME_BY_LBA(0x00061980, 5751263, BrxWorld, "f_nightclub_moulinrouge"),
	NAME_BY_LBA(0x00063240, 4908636, BrxWorld, "f_nightclub_waterpump"),
	NAME_BY_LBA(0x000678a0, 11689480, BrxWorld, "i_palace_ext"),
	NAME_BY_LBA(0x0006bd40, 7786626, BrxWorld, "i_palace_int"),
	NAME_BY_LBA(0x0006dfe0, 6104199, BrxWorld, "i_palace_basement"),
	NAME_BY_LBA(0x00070c00, 6354785, BrxWorld, "i_palace_ballroom"),
	NAME_BY_LBA(0x00075120, 12361630, BrxWorld, "i_temple_ext"),
	NAME_BY_LBA(0x0007b7e0, 7747866, BrxWorld, "i_temple_int"),
	NAME_BY_LBA(0x0007e7e0, 11383853, BrxWorld, "p_prison_ext"),
	NAME_BY_LBA(0x00083160, 7119379, BrxWorld, "p_prison_int"),
	NAME_BY_LBA(0x000862a0, 3737282, BrxWorld, "p_prison_vault"),
	NAME_BY_LBA(0x00088400, 11111409, BrxWorld, "p_castle_ext"),
	NAME_BY_LBA(0x0008c880, 4208012, BrxWorld, "p_castle_waterrails"),
	NAME_BY_LBA(0x00092960, 5363039, BrxWorld, "p_castle_guardbreak"),
	NAME_BY_LBA(0x00095720, 4588804, BrxWorld, "p_castle_wolftomb"),
	NAME_BY_LBA(0x0008f3c0, 7234451, BrxWorld, "p_castle_heist"),
	NAME_BY_LBA(0x00098360, 4456544, BrxWorld, "p_castle_ewoktrainer"),
	NAME_BY_LBA(0x0009dac0, 4500407, BrxWorld, "p_castle_ewoktomb"),
	NAME_BY_LBA(0x0009af40, 4273600, BrxWorld, "p_castle_ewokhall"),
	NAME_BY_LBA(0x000a06c0, 4155295, BrxWorld, "p_castle_ewokwater"),
	NAME_BY_LBA(0x000a3200, 9734627, BrxWorld, "c_train_ext"),
	NAME_BY_LBA(0x000a70e0, 5117219, BrxWorld, "c_train_cabins"),
	NAME_BY_LBA(0x000a9d80, 6442506, BrxWorld, "c_train_a"),
	NAME_BY_LBA(0x000acae0, 6689122, BrxWorld, "c_train_b"),
	NAME_BY_LBA(0x000af240, 6260457, BrxWorld, "c_train_c"),
	NAME_BY_LBA(0x000b18c0, 10965082, BrxWorld, "c_sawmill_ext"),
	NAME_BY_LBA(0x000b6260, 4755428, BrxWorld, "c_sawmill_mulch"),
	NAME_BY_LBA(0x000b92a0, 5074218, BrxWorld, "c_sawmill_burn"),
	NAME_BY_LBA(0x000bbf60, 4974075, BrxWorld, "c_sawmill_lighthouse"),
	NAME_BY_LBA(0x000bec00, 3715670, BrxWorld, "c_sawmill_bearcave"),
	NAME_BY_LBA(0x000c1600, 4752435, BrxWorld, "c_sawmill_boss"),
	NAME_BY_LBA(0x000c4860, 9793104, BrxWorld, "a_blimp_ext"),
	NAME_BY_LBA(0x000c8300, 6186356, BrxWorld, "a_blimp_int"),
	NAME_BY_LBA(0x000cdb00, 4904282, BrxWorld, "a_blimp_engine_room_murray"),
	NAME_BY_LBA(0x000cb100, 4317986, BrxWorld, "a_blimp_engine_room_bentley"),
	NAME_BY_LBA(0x000d06a0, 4271749, BrxWorld, "a_blimp_engine_room_jt"),
	NAME_BY_LBA(0x000d30a0, 9209356, BrxWorld, "a_blimp_boss_final"),
END_NAME_MAPPING_TABLE();

MAKE_RELEASE(NtscU, Release, 0x10f, 0x1a32e8, 30, 3232);
