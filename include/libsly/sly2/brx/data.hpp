#pragma once

#include <libsly/sly2/brx/proxy_table.hpp>
#include <libsly/sly2/brx/save_table.hpp>
#include <libsly/sly2/brx/sound.hpp>
#include <libsly/sly2/brx/world_table.hpp>
#include <libsly/sly2/brx/text.hpp>
#include <vector>

namespace sly::sly2::brx {

	/// All parsed BRX data laid out into a semi-competent struct.
	struct BrxData {
		std::vector<ProxyTableEntry> proxyTable;
		SoundData sound;
		SaveTableData save;
		WorldTable worldTable;
		Text text;
	};

} // namespace sly::sly2::brx
