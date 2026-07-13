import std.io;

namespace Common {

	// These are obfuscated catalog entries.
	// The Retail sly 2 builds use this
	// instead of WAL FK$ names.
	struct CdCatalogEntry {
		u32 isector;
		u32 icb;
		u32 iid;

		padding[40]; // Junk data.

		// XOR "decryption" keys.
		u32 sectorkey;
		u32 idkey;
		u32 cbkey;

		std::print("0x{:08x}, {}", ((isector ^ sectorkey)+3232) * 0x800, icb ^ cbkey);
	} [[single_color,format("Common::format_discloc")]];

	fn format_discloc(ref CdCatalogEntry loc) {
		return std::format("0x{:08x}", ((loc.isector ^ loc.sectorkey) + 3232) * 0x800);
	};

	struct SwString {
		s16 stringLen;
		char string[stringLen];
	} [[single_color, format("Common::format_swstring")]];

	fn format_swstring(ref SwString string) {
		return std::format("{}", string.string);
	};

}

// In order of how data is read, where possible.

namespace ProxyTable {
	struct Entry {
		char name[0x40];
		u32 unk1;
		u32 unk2;
		u32 oid;
		u32 unk4;
		u32 unk5;
	};
}

namespace CSoundMgr {

	struct Unk6FileLocTable {
		Common::CdCatalogEntry fiVag; // points to VAGp data
		s8 unk;
		float unk2;
	};

	struct Unk6IntTable {
		s16 unk;
		s16 unk2;
		s32 unk3;
	};

	struct Unk6 {
		s16 unk1;
		s16 unk2;
		CSoundMgr::Unk6FileLocTable table[parent.cUnk5];
		s32 cTable;
		CSoundMgr::Unk6IntTable aTable[cTable];
	};

	struct Unk7 {
		s16 unk;
		Common::CdCatalogEntry fiUnk;
	};

	struct Root {
		Common::CdCatalogEntry fiBank; // points to 989snd bank data

		s16 cUnk1;
		s8 aUnk2[cUnk1];

		s16 cUnk2;
		s16 aUnk3[cUnk1]; // game array is sized as cUnk2,
						// but CSoundMgr::LoadFromBrx reads cUnk1

		s16 cUnk4;
		if(cUnk4 > 0) {
			Common::CdCatalogEntry fiVpk; // points to 'VPK ' data
			s16 aUnk4[cUnk4];
		}

		u8 cUnk5;
		u8 aUnk5[cUnk5];

		s32 cUnk6;
		CSoundMgr::Unk6 aUnk6[cUnk6];

		s32 cUnk7;
		CSoundMgr::Unk7 aUnk7[cUnk7];
	};

}

namespace CSaveTable {

	struct SaveHashTableEntry {
		u16 nValue;
		Common::SwString sKey;
	};

	struct Unkentry {
		u8 unk1;
		u8 unk2;
		u8 unk3;
		u8 unk4;
		u8 unk5;
		u8 unk6;
		u8 unk7;
		u8 unk8;
		u8 unk9;
		u8 unka;
	};

	struct Root {
		u16 nHashSize;
		CSaveTable::SaveHashTableEntry entries[nHashSize];

		u16 nUnkSize;
		CSaveTable::Unkentry aUnk[nUnkSize];
	};

}

namespace WorldTable {

	struct Entry {
		Common::CdCatalogEntry loc;
		u8 unk1;
		u8 unk2;
		u8 unk3;
		u8 cUnk;
		s16 aUnk[cUnk];
	};

	struct Root {
		u32 cWorlds;
		u32 cUnk2;
		std::print("WORLDTABLE");
		WorldTable::Entry aWorlds[cWorlds];
	};

}

namespace TextTable {

	struct StringEntry {
		s16 id;
		Common::SwString string;
	} [[format("TextTable::format_string_entry")]];

	fn format_string_entry(ref StringEntry se) {
		return std::format("{}", se.string.string);
	};

	struct Root {
		u8 unk1;
		u8 unk2;

		s16 nrStrings;
		TextTable::StringEntry strings[nrStrings];
	};
}

namespace UnkTable {

	struct node {
		float a;
		float b;
		float c;
		float d;
	};

	struct Entry {
		node node;
		s16 u1; // could be backlink?
		s16 u2; // could be frontlink?
	};

	struct Root {
		u16 cUnk;
		Entry entry[cUnk];
	};

}

struct Brx {
	s16 cProxy;
	ProxyTable::Entry aProxy[cProxy];

	// Class init datas
	CSoundMgr::Root CSoundMgr;
	CSaveTable::Root CSaveTable;

	// World Table
	WorldTable::Root WorldTable;
	TextTable::Root TextTable;

	UnkTable::Root UnkTable;
};


Brx brx @ 0;
