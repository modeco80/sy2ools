#pragma once
#include <libsly/core/sly_lz_stream.hpp>
#include <libsly/sly2/archive_filesystem.hpp>
#include <libsly/sly2/brx/data.hpp>
#include <libsly/sly2/brx/object.hpp>
#include <libsly/sly2/brx/option.hpp>
#include <libsly/sly2/file_location.hpp>

namespace sly::sly2 {
	namespace brx {
		struct OptionMap;

		/// BRX level parser.
		class Parser {
			IArchiveFileSystem& fs;
			const OptionMap* optionMap;
			mco::Stream* brxRawStream;
			std::unique_ptr<sly::core::SlyLzStream> brxStream;

			// Parse functions.
			//
			// NOTE:
			// These are private as the brx data is essentially individual types of data
			// that is merged into a single compressed byte stream.
			//
			// Therefore, it makes no real sense to parse *only* one type of data.
			//
			// Something which needs only one kind of data
			// can move out the data it wants out of the BrxData struct or only use that data.

			bool parseProxyTable(BrxData& data);
			bool parseSoundData(BrxData& data);
			bool parseSaveTable(BrxData& data);
			bool parseWorldTable(BrxData& data);
			bool parseText(BrxData& data);
			bool parseUnkBsp(BrxData& data);

		   public:
			// helper which opens from a IArchiveFileSystem instance
			Parser(IArchiveFileSystem& fs, const FileLocation& loc);
			~Parser();

			// TODO (unfortunately): mco::Stream& getStream() const;

			bool parseOption(const OptionDescriptor* option, OptionValue& reciever);
			bool parseOptions(OptionList& options);

			/// Parses all data out of the brx level..
			bool parseAll(BrxData& data);
		};

	} // namespace brx
} // namespace sly::sly2
