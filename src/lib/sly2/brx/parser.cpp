#include <libsly/sly2/brx/parser.hpp>
#include <libsly/sly2/iso_filesystem.hpp>

#include "io_utils.hpp"
#include "options.hpp"

namespace sly::sly2::brx {

	Parser::Parser(IArchiveFileSystem& fs, const FileLocation& loc)
		: fs(fs) {
		// Open the raw file from the given archive implementation.
		// If this fails, we can't really do much else anyways.
		brxRawStream = fs.openFile(loc);
		if(brxRawStream == nullptr) {
			throw std::runtime_error("Could not open BRX from given IArchiveFileSystem implementation.");
		}

		optionMap = getOptionMapForRelease(fs.getRelease());
		if(optionMap == nullptr) {
			throw std::runtime_error("Option map is nullptr? How?");
		}

		// Create the LZ decompression stream on top of the raw file stream.
		brxStream = std::make_unique<sly::core::SlyLzStream>(*brxRawStream);
	}

	Parser::~Parser() {
		brxStream.reset();
		fs.closeFile(brxRawStream);
	}

	bool Parser::parseProxyTable(BrxData& data) {
		try {
			const auto countProxyTable = readLiteral<i16>(*brxStream);
			data.proxyTable.resize(countProxyTable);

			// Read every proxy table item.
			for(auto i = 0; i < countProxyTable; ++i) {
				// This is supposed to be a location. However it was never mapped
				// to a CD catalog entry, since it didn't exist in the WAL.
				readAll(*brxStream, &data.proxyTable[i].name[0], 0x40);
				data.proxyTable[i].unk1 = readLiteral<i32>(*brxStream);
				data.proxyTable[i].unk2 = readLiteral<i32>(*brxStream);
				data.proxyTable[i].oid = readLiteral<i32>(*brxStream);
				data.proxyTable[i].unk4 = readLiteral<i32>(*brxStream);
				data.proxyTable[i].unk5 = readLiteral<i32>(*brxStream);
			}
		} catch(ShortRead&) {
			data.proxyTable.clear();
			return false;
		}

		return true;
	}

	bool Parser::parseSoundData(BrxData& data) {
		auto& snd = data.sound;

		auto parseVoiceLine = [&](VoiceLine& line, u32 cLanguages) {
			line.unk1 = readLiteral<i16>(*brxStream);
			line.unk2 = readLiteral<i16>(*brxStream);

			line.languages.resize(cLanguages);

			for(auto i = 0; i < cLanguages; ++i) {
				line.languages[i].vagLocation = readLocation(*brxStream, fs);
				line.languages[i].unk = readLiteral<i8>(*brxStream);
				line.languages[i].flRuntime = readLiteral<float>(*brxStream);
			}

			const auto cTable = readLiteral<i32>(*brxStream);
			line.table.resize(cTable);
			for(auto i = 0; i < cTable; ++i) {
				line.table[i].unk = readLiteral<i16>(*brxStream);
				line.table[i].unk2 = readLiteral<i16>(*brxStream);
				line.table[i].unk3 = readLiteral<i32>(*brxStream);
			}
		};

		try {
			snd.bankFile = readLocation(*brxStream, fs);

			const auto cUnkTable1 = readLiteral<i16>(*brxStream);
			snd.unkTable.resize(cUnkTable1);
			for(auto i = 0; i < cUnkTable1; ++i)
				snd.unkTable[i] = readLiteral<i8>(*brxStream);

			const auto cUnkTable2 = readLiteral<i16>(*brxStream);
			snd.unkTable2.resize(cUnkTable2, -1);
			for(auto i = 0; i < cUnkTable1; ++i)
				snd.unkTable2[i] = readLiteral<i16>(*brxStream);

			const auto cEffects = readLiteral<i16>(*brxStream);
			if(cEffects > 0) {
				snd.effectData = readLocation(*brxStream, fs);

				snd.effects.resize(cEffects);
				for(auto i = 0; i < cEffects; ++i)
					snd.effects[i] = readLiteral<i16>(*brxStream);
			}

			const auto cLanguages = readLiteral<u8>(*brxStream);

			snd.languageIdTable.resize(cLanguages);
			for(auto i = 0; i < cLanguages; ++i)
				snd.languageIdTable[i] = readLiteral<u8>(*brxStream);

			const auto cVoiceLines = readLiteral<i32>(*brxStream);
			snd.voiceLines.resize(cVoiceLines);
			for(auto i = 0; i < cVoiceLines; ++i)
				parseVoiceLine(snd.voiceLines[i], cLanguages);

			const auto cMusic = readLiteral<i32>(*brxStream);
			snd.music.resize(cMusic);
			for(auto i = 0; i < cMusic; ++i) {
				snd.music[i].unk = readLiteral<i16>(*brxStream);
				snd.music[i].musicFile = readLocation(*brxStream, fs);
			}
		} catch(ShortRead&) {
			return false;
		}

		return true;
	}

	bool Parser::parseSaveTable(BrxData& data) {
		auto& save = data.save;
		try {
			const auto cHashTable = readLiteral<u16>(*brxStream);
			save.hashTableEntries.resize(cHashTable);
			for(auto i = 0; i < cHashTable; ++i) {
				save.hashTableEntries[i].value = readLiteral<u16>(*brxStream);
				save.hashTableEntries[i].key = readSwString(*brxStream);
			}

			const auto cUnkTable = readLiteral<u16>(*brxStream);
			save.unkTable.resize(cUnkTable);
			for(auto i = 0; i < cUnkTable; ++i) {
				save.unkTable[i].m1 = readLiteral<i16>(*brxStream);
				save.unkTable[i].m2 = readLiteral<u8>(*brxStream);
				save.unkTable[i].m3 = readLiteral<u8>(*brxStream);
				save.unkTable[i].m4 = readLiteral<i16>(*brxStream);
				save.unkTable[i].m5 = readLiteral<i16>(*brxStream);
				save.unkTable[i].m6 = readLiteral<i16>(*brxStream);
			}
		} catch(ShortRead&) {
			return false;
		}

		return true;
	}

	bool Parser::parseWorldTable(BrxData& data) {
		auto& worldTable = data.worldTable;

		try {
			const auto cWorlds = readLiteral<u32>(*brxStream);
			worldTable.worlds.resize(cWorlds);

			worldTable.unk = readLiteral<u32>(*brxStream);

			for(auto i = 0; i < cWorlds; ++i) {
				worldTable.worlds[i].locBrx = readLocation(*brxStream, fs);
				worldTable.worlds[i].unk1 = readLiteral<u8>(*brxStream);
				worldTable.worlds[i].unk2 = readLiteral<u8>(*brxStream);
				worldTable.worlds[i].unk3 = readLiteral<u8>(*brxStream);

				const auto cUnk = readLiteral<u8>(*brxStream);
				worldTable.worlds[i].unk.resize(cUnk);
				for(auto j = 0; j < cUnk; ++j)
					worldTable.worlds[i].unk[j] = readLiteral<i16>(*brxStream);
			}
		} catch(ShortRead&) {
			return false;
		}

		return true;
	}

	bool Parser::parseText(BrxData& data) {
		auto& text = data.text;

		try {
			const auto cLanguages = readLiteral<u8>(*brxStream);
			text.languageIds.resize(cLanguages);
			for(auto i = 0; i < cLanguages; ++i) {
				text.languageIds[i] = readLiteral<u8>(*brxStream);
			}

			const auto cStrings = readLiteral<i16>(*brxStream);
			text.stringTable.resize(cStrings);
			for(auto i = 0; i < cStrings; ++i) {
				text.stringTable[i].stringId = readLiteral<i16>(*brxStream);
				text.stringTable[i].strings.resize(cLanguages);
				for(auto j = 0; j < cLanguages; ++j)
					text.stringTable[i].strings[j] = readSwString(*brxStream);
			}
		} catch(ShortRead&) {
			return false;
		}

		return true;
	}


	bool Parser::parseUnkBsp(BrxData& data) {
		auto& unkBsp = data.unkBspData;

		try {
			const auto cEntries = readLiteral<i16>(*brxStream);
			unkBsp.entries.resize(cEntries);
			for(auto i = 0; i < cEntries; ++i) {
				unkBsp.entries[i].f1 = readLiteral<float>(*brxStream);
				unkBsp.entries[i].f2 = readLiteral<float>(*brxStream);
				unkBsp.entries[i].f3 = readLiteral<float>(*brxStream);
				unkBsp.entries[i].f4 = readLiteral<float>(*brxStream);
				const auto backlink = readLiteral<i16>(*brxStream);
				if(backlink > 0) {
					unkBsp.entries[i].linkleft = &unkBsp.entries[backlink];
				}

				const auto frontlink = readLiteral<i16>(*brxStream);
				if(frontlink > 0) {
					unkBsp.entries[i].linkright = &unkBsp.entries[frontlink];
				}
			}
		} catch(ShortRead&) {
			return false;
		}

		return true;
	}

	bool Parser::parseOption(const OptionDescriptor* option, OptionValue& reciever) {
		try {
			switch(option->type) {
				case OptionType::Bool: {
					auto bVal = readLiteral<u8>(*brxStream);
					reciever = OptionValue(static_cast<bool>(bVal));
					return true;
				};

				case OptionType::Float: {
					auto fVal = readLiteral<float>(*brxStream);
					reciever = OptionValue(fVal);
					return true;
				};

				case OptionType::Vector: {
					auto vVal = readLiteral<sly2::Vector>(*brxStream);
					reciever = OptionValue(vVal);
					return true;
				};

				case OptionType::Smpa: {
					reciever = OptionValue(readLiteral<SmoothingParameters>(*brxStream));
					return true;
				};

				case OptionType::Rgba: {
					reciever = OptionValue(readLiteral<Rgba>(*brxStream));
					return true;
				};


				case OptionType::Wid: {
					auto wid = readLiteral<Wid>(*brxStream);
					reciever = OptionValue(wid);
					return true;
				};


				// don't think we need to do anything for this
				case OptionType::Void:
					reciever = OptionValue();
					return true;

				default:
					return false;
			}
		} catch(ShortRead&) {
			return false;
		}
	}

	bool Parser::parseOptions(OptionList& options) {
		while(true) {
			auto id = readLiteral<i16>(*brxStream);
			// End of the options list.
			if(id < 0)
				break;

			const auto* desc = optionMap->find(id);

			if(desc == nullptr) {
				return false;
			}

			OptionValue value;
			if(!parseOption(desc, value))
				return false;

			options.addOption(desc, value);
		}
		return true;
	}

	bool Parser::parseAll(BrxData& data) {
		data.worldObject.name = "WORLD";
		data.cameraObject.name = "CAMERA";

		if(!parseProxyTable(data))
			return false;
		if(!parseSoundData(data))
			return false;
		if(!parseSaveTable(data))
			return false;
		if(!parseWorldTable(data))
			return false;
		if(!parseText(data))
			return false;
		if(!parseUnkBsp(data))
			return false;

		// World object options
		if(!parseOptions(data.worldObject.options))
			return false;

		// Camera object options
		if(!parseOptions(data.cameraObject.options))
			return false;

		return true;
	}

} // namespace sly::sly2::brx
