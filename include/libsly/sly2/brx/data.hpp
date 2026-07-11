#pragma once

#include <vector>
#include <libsly/sly2/brx/proxy_table.hpp>

namespace sly::sly2::brx {

		struct BrxData {

			// Brx data.
			std::vector<ProxyTableEntry> proxyTable;

		};

}
