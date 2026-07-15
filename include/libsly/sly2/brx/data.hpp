#pragma once

#include <libsly/sly2/brx/proxy_table.hpp>
#include <libsly/sly2/brx/save_table.hpp>
#include <libsly/sly2/brx/sound.hpp>
#include <libsly/sly2/brx/text.hpp>
#include <libsly/sly2/brx/world_table.hpp>
#include <libsly/sly2/brx/unk_bsp.hpp>
#include <vector>
#include <libsly/sly2/brx/object.hpp>

namespace sly::sly2::brx {

	/// All parsed BRX data laid out into a semi-competent struct.
	struct BrxData {
		std::vector<ProxyTableEntry> proxyTable;
		SoundData sound;
		SaveTableData save;
		WorldTable worldTable;
		Text text;
		UnkBspData unkBspData;

		Object worldObject;
		Object cameraObject;
	};

} // namespace sly::sly2::brx
