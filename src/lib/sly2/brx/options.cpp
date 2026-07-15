#include "options.hpp"

#include <cstring>
#include <libsly/sly2/brx/option.hpp>

namespace sly::sly2::brx {

	DECLARE_OPTION(WID, "WID", false, OptionType::Wid);
	DECLARE_OPTION(SHADOW_HSV, "SHADOW_HSV", false, OptionType::Vector);

	// The option map solves a bit of an interesting.. "problem". Different builds of
	// Sly 2 may have different option indicies. However, the option data type itself
	// may not actually differ. Therefore, we use this OptionDescriptor/OptionMap structure
	// to actually do so.
	BEGIN_OPTION_MAP(NtscU, Release)
	OPTION_MAP(913, WID),
	OPTION_MAP(914, SHADOW_HSV),
	END_OPTION_MAP(NtscU, Release);

	const OptionDescriptor* OptionMap::find(i16 opId) const {
		// a meh-quality linear search.
		for(auto i = 0; i < nOptions; ++i)
			if(pOptions[i].optionId == opId)
				return pOptions[i].option;
		return nullptr;
	}

	const OptionDescriptor* OptionMap::find(const char* pszName) const {
		for(auto i = 0; i < nOptions; ++i)
			if(!std::strcmp(pOptions[i].option->pszName, pszName))
				return pOptions[i].option;
		return nullptr;
	}

	const OptionMap* getOptionMapForRelease(Release release) {
#define X(region, version, _sha256) \
	case region##_##version:        \
		return &sOptionMap_##region##_##version;
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
} // namespace sly::sly2::brx
