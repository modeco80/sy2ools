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

	struct VoiceLineLanguage {
		Common::CdCatalogEntry catVag; // points to VAGp data for the clip
		s8 unk;
		float flRuntime; // clip length
	};

	struct VoiceLineUnkTable {
		s16 unk;
		s16 unk2;
		s32 unk3;
	};

	struct VoiceLine {
		s16 unk1;
		s16 unk2;
		CSoundMgr::VoiceLineLanguage voiceLines[parent.cLanguages];
		s32 cTable;
		CSoundMgr::VoiceLineUnkTable aTable[cTable];
	};

	struct Music {
		s16 unk;
		Common::CdCatalogEntry fiMusic;
	};

	struct Root {
		Common::CdCatalogEntry catBank; // points to 989snd bank data

		s16 cUnk1;
		s8 aUnk2[cUnk1];

		s16 cUnk2;
		s16 aUnk3[cUnk1]; // game array is sized as cUnk2,
						// but CSoundMgr::LoadFromBrx reads cUnk1

		s16 cEffects;
		if(cEffects > 0) {
			Common::CdCatalogEntry catEffects; // points to 'VPK ' data
			s16 aEffects[cEffects];
		}

		u8 cLanguages;
		u8 aLanguageIdTable[cLanguages]; //?

		s32 cVoiceLines;
		CSoundMgr::VoiceLine aVoiceLines[cVoiceLines];

		s32 cMusic;
		CSoundMgr::Music aMusic[cMusic];
	};

}

namespace CSaveTable {

	struct SaveHashTableEntry {
		u16 nValue;
		Common::SwString sKey;
	};

	struct Unkentry {
		s16 m1;
		u8 m2;
		u8 m3;
		s16 m4;
		s16 m5;
		s16 m6;
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
		Common::SwString strings[parent.cLanguages];
	} [[format("TextTable::format_string_entry")]];

	fn format_string_entry(ref StringEntry se) {
		return std::format("{}", se.strings[0].string);
	};

	struct Root {
		u8 cLanguages;
		u8 languageIdTable[cLanguages];

		s16 cStrings;
		TextTable::StringEntry aStrings[cStrings];
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

	// Classes which have ::LoadFromBrx() methods
	CSoundMgr::Root CSoundMgr;
	CSaveTable::Root CSaveTable;

	// World Table
	WorldTable::Root WorldTable;
	TextTable::Root TextTable;

	UnkTable::Root UnkTable;
};


Brx brx @ 0;
