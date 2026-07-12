#pragma once
#include <libsly/core/sly_lz_stream.hpp>
#include <libsly/sly2/archive_filesystem.hpp>
// Brx data.
#include <libsly/sly2/brx/data.hpp>

namespace sly::sly2 {
	namespace brx {

		/// BRX level parser.
		class Parser {
			IArchiveFileSystem& fs;
			mco::Stream* brxRawStream;
			std::unique_ptr<sly::core::SlyLzStream> brxStream;

			// individual parse functions. These are private as the brx data is essentially
			// individual types of data merged into one byte stream. Therefore, it makes no sense
			// to parse *only* one type of data. Something which needs only one kind of data
			// can move out the data it wants out of the BrxData struct or only use that data.
			bool parseProxyTable(BrxData& data);

		   public:
			Parser(IArchiveFileSystem& fs, const char* pszFileName);
			~Parser();

			/// Parses all data out of the brx level..
			bool parseAll(BrxData& data);
		};

	} // namespace brx
} // namespace sly::sly2
