#pragma once
#include <libsly/sly2/file_location.hpp>
#include <mco/base_types.hpp>
#include <optional>
#include <vector>

namespace sly::sly2::brx {

	struct VoiceLine {
		struct LineFile {
			FileLocation vagLocation;
			int unk;
			float unk2;
		};

		struct UnkTable {
			int unk;
			int unk2;
			int unk3;
		};

		int unk1;
		int unk2;
		std::vector<LineFile> lineFiles;
		std::vector<UnkTable> table;
	};

	struct MusicEntry {
		int unk;
		FileLocation musicFile;
	};

	struct SoundData {
		FileLocation bankFile;

		std::vector<int> unkTable;
		std::vector<int> unkTable2;

		std::vector<int> effects;
		std::optional<FileLocation> effectData;

		std::vector<u8> languageIdTable;

		std::vector<VoiceLine> voiceLines;

		std::vector<MusicEntry> music;
	};

} // namespace sly::sly2::brx
