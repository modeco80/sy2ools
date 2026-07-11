#include <libsly/sly2/iso_filemapping.hpp>
#include <libsly/sly2/release.hpp>

namespace sly::sly2 {

	// clang-format off

// This declares the name mapping table. This maps both catalog files (files in the ELF's internal catalog)
// and files accessed by bolted catalog entries to "sane" filenames. Once this is completely filled out,
// in theory a "rewal" too may be possible to make which re-inserts WAL FK$ entries.
BEGIN_NAME_MAPPING_TABLE(NtscU, Release)
	// Catalog names
	NAME_BY_FID(24, FileKind::PssMovie, "splash.pss"),
	NAME_BY_FID(25, FileKind::BrxMap, "intro.brx"),
	NAME_BY_FID(26, FileKind::IconMemcard, "memcard_icon.ico")
	// CD Sector names.
END_NAME_MAPPING_TABLE();

MAKE_RELEASE(NtscU, Release, 0x10f, 0x1a32e8, 30, 3232);
	// clang-format on

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
