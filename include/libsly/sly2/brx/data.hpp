#pragma once

#include <libsly/sly2/brx/proxy_table.hpp>
#include <vector>

namespace sly::sly2::brx {

	struct BrxData {
		// Brx data.
		std::vector<ProxyTableEntry> proxyTable;
	};

} // namespace sly::sly2::brx
