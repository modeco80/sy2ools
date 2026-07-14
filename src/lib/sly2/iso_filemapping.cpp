#include <libsly/sly2/iso_filemapping.hpp>
#include <libsly/sly2/release.hpp>

#include "isomappings/ntscu_retail.hpp"

namespace sly::sly2 {

	const ReleaseCdDataTable* getCdDataForRelease(Release release) {
#define X(region, version, _sha256) \
	case region##_##version:        \
		return &sCatalogEntry_##region##_##version;
		using enum Release;
		switch(release) {
			LIBSLY_SLY2_RELEASES()
			// Invalid or no mapping table created.
			case Invalid:
			default:
				return nullptr;
		}
#undef RELEASE
	}

} // namespace sly::sly2
