#pragma once
#include <libsly/sly2/file_location.hpp>
#include <mco/base_types.hpp>
#include <optional>
#include <vector>

namespace sly::sly2::brx {

	struct VoiceLine {
		struct LineLanguage {
			FileLocation vagLocation;
			int unk;
			/// Runtime of the clip.
			float flRuntime;
		};

		struct UnkTable {
			int unk;
			int unk2;
			int unk3;
		};

		int unk1;
		int unk2;
		/// Each language's take.
		std::vector<LineLanguage> languages;
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
