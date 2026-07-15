#pragma once
#include <mco/base_types.hpp>
#include <vector>

namespace sly::sly2::brx {

	struct UnkBspData {
		struct Entry {
			float f1;
			float f2;
			float f3;
			float f4;

			Entry* linkleft;
			Entry* linkright;
		};

		std::vector<Entry> entries;
	};

} // namespace sly::sly2::brx
