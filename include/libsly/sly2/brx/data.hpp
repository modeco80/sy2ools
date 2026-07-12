#pragma once

#include <libsly/sly2/brx/proxy_table.hpp>
#include <vector>

namespace sly::sly2::brx {

	/// All parsed BRX data laid out into a semi-competent struct.
	struct BrxData {
		std::vector<ProxyTableEntry> proxyTable;
	};

} // namespace sly::sly2::brx
