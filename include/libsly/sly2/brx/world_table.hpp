
#pragma once

#include <libsly/sly2/file_location.hpp>
#include <vector>

namespace sly::sly2::brx {

	struct WorldTable {
		struct Entry {
			FileLocation locBrx; // Location to the BRX for this world
			u32 unk1;
			u32 unk2;
			u32 unk3;
			std::vector<int> unk;
		};

		u32 unk;
		std::vector<Entry> worlds;
	};

} // namespace sly::sly2::brx
