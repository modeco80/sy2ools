#pragma once
#include <vector>
#include <string>
#include <mco/base_types.hpp>

namespace sly::sly2::brx {

	struct SaveTableData {
		struct HashTableEntry {
			std::string key;
			u32 value;
		};

		struct UnkEntry {
			i16 m1;
			u8 m2;
			u8 m3;
			i16 m4;
			i16 m5;
			i16 m6;
		};

		std::vector<HashTableEntry> hashTableEntries;
		std::vector<UnkEntry> unkTable;
	};

}
