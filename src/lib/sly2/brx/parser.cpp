#include <libsly/sly2/brx/parser.hpp>
#include <libsly/sly2/iso_filesystem.hpp>

namespace sly::sly2::brx {

	// IO helpers
	// TODO: might be useful in another file.

	class ShortRead : public std::exception {
	   public:
		const char* what() const noexcept override {
			return "short read";
		}
	};

	void readAll(mco::Stream& stream, void* pBuf, usize count) {
		if(stream.read(pBuf, count) != count)
			throw ShortRead();
	}

	template <class T>
	T readLiteral(mco::Stream& stream) {
		T item;
		readAll(stream, &item, sizeof(T));
		return item;
	}

	std::string readSwString(mco::Stream& stream) {
		std::string ret;
		const auto nStringLen = readLiteral<i16>(stream);
		ret.resize(nStringLen);
		readAll(stream, &ret[0], nStringLen);
		return ret;
	}

	template <class TCountInt, class Out, class FnRead>
	void readArray(mco::Stream& stream, std::vector<Out>& array, FnRead&& doRead) {
		const auto nCount = readLiteral<TCountInt>(stream);
		array.resize(nCount);
		for(usize i = 0; i < nCount; ++i)
			doRead(stream, array[i]);
	}

	FileLocation readLocation(mco::Stream& stream, IArchiveFileSystem& fs) {
		if(fs.getKind() == IsoFileSystem::Kind) {
			auto& isoFs = reinterpret_cast<IsoFileSystem&>(fs);
			CdCatalogEntry catEnt;
			readAll(stream, &catEnt, sizeof(CdCatalogEntry));

			// IsoFileSystem contains a helper method to map CD catalogs back to
			// FK$ if it knows about them, so use it. This allows locations read here
			// to become FK$ as soon as the mappings are updated, which is pretty cool.
			return isoFs.mapCatalogEntryToName(catEnt);
		} else {
			// FK$ lookup string.
			char bufLoc[0x40] {};
			readAll(stream, &bufLoc[0], sizeof(bufLoc));
			return FileLocation(bufLoc);
		}
	}

	Parser::Parser(IArchiveFileSystem& fs, const char* pszFileName)
		: fs(fs) {
		// Open the raw file from the given archive implementation.
		// If this fails, we can't really do much else anyways.
		brxRawStream = fs.openFile(pszFileName);
		if(brxRawStream == nullptr) {
			throw std::runtime_error("Could not open BRX from given IArchiveFileSystem implementation.");
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

	bool Parser::parseAll(BrxData& data) {
		if(!parseProxyTable(data))
			return false;
		if(!parseSoundData(data))
			return false;
		if(!parseSaveTable(data))
			return false;
		if(!parseWorldTable(data))
			return false;
		return true;
	}

} // namespace sly::sly2::brx
