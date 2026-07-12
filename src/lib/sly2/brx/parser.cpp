#include <libsly/sly2/brx/parser.hpp>

namespace sly::sly2::brx {

	// IO helpers

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

	bool Parser::parseAll(BrxData& data) {
		if(!parseProxyTable(data))
			return false;

		return true;
	}

} // namespace sly::sly2::brx
