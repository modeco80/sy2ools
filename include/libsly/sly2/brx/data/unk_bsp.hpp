#pragma once
#include <mco/base_types.hpp>
#include <vector>

namespace sly::sly2::brx {

	// TODO: This is most likely bsp related?
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
