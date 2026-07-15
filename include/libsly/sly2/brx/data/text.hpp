#pragma once

#include <mco/base_types.hpp>
#include <string>
#include <vector>

namespace sly::sly2::brx {

	struct Text {
		struct StringTableEntry {
			int stringId;
			std::vector<std::string> strings;
		};

		std::vector<int> languageIds;
		std::vector<StringTableEntry> stringTable;
	};

} // namespace sly::sly2::brx
