#include <libsly/sly2/iso_filemapping.hpp>
#include <libsly/sly2/release.hpp>

namespace sly::sly2 {

	// clang-format off

BEGIN_NAME_MAPPING_TABLE(NtscU, Release)
	// Catalog names
	NAME_BY_FID(24, "splash.pss"),
	NAME_BY_FID(25, "intro.brx"),
	NAME_BY_FID(26, "memcard_icon.ico")
	// CD Sector names.
END_NAME_MAPPING_TABLE();

MAKE_RELEASE(NtscU, Release, 0x10f, 0x1a32e8, 30, 3232);
	// clang-format on

	const ReleaseCdMappingTable* getCatalogEntryForRelease(Release release) {
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
