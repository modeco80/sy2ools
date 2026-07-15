#pragma once

#include <libsly/sly2/brx/data/proxy_table.hpp>
#include <libsly/sly2/brx/data/save_table.hpp>
#include <libsly/sly2/brx/data/sound.hpp>
#include <libsly/sly2/brx/data/text.hpp>
#include <libsly/sly2/brx/data/world_table.hpp>
#include <libsly/sly2/brx/data/unk_bsp.hpp>
#include <vector>
#include <libsly/sly2/brx/objects/object.hpp>

namespace sly::sly2::brx {

	/// All parsed BRX data laid out into a semi-competent struct.
	struct BrxData {
		std::vector<ProxyTableEntry> proxyTable;
		SoundData sound;
		SaveTableData save;
		WorldTable worldTable;
		Text text;
		UnkBspData unkBspData;

		std::unique_ptr<Object> worldObject;
		std::unique_ptr<Object> cameraObject;

	};

} // namespace sly::sly2::brx
