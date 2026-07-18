#include "options.hpp"

#include <cstring>
#include <libsly/sly2/option.hpp>

namespace sly::sly2::brx {

	DECLARE_OPTION(WID, "WID", false, OptionType::Wid);
	DECLARE_OPTION(SHADOW_HSV, "SHADOW_HSV", false, OptionType::Vector);
	DECLARE_OPTION(AMBIENT_MIDTONE, "AMBIENT_MIDTONE", false, OptionType::Float);
	DECLARE_OPTION(AMBIENT_SHADOW, "AMBIENT_SHADOW", false, OptionType::Float);

	// CCm
	DECLARE_OPTION(FIELD_OF_VIEW, "FIELD_OF_VIEW", false, OptionType::Float);
	DECLARE_OPTION(FIELD_OF_VIEW_TARGET, "FIELD_OF_VIEW_TARGET", false, OptionType::Float);
	DECLARE_OPTION(FIELD_OF_VIEW_SMOOTHING, "FIELD_OF_VIEW_TARGET", false, OptionType::Smpa);
	DECLARE_OPTION(NEAR_CLIP, "NEAR_CLIP", false, OptionType::Float);
	DECLARE_OPTION(FAR_CLIP, "FAR_CLIP", false, OptionType::Float);
	DECLARE_OPTION(NEAR_FOG, "NEAR_FOG", false, OptionType::Float);
	DECLARE_OPTION(FAR_FOG, "FAR_FOG", false, OptionType::Float);
	DECLARE_OPTION(FOG_COLOR, "FOG_COLOR", false, OptionType::Rgba);
	DECLARE_OPTION(FOG_MAX, "FOG_MAX", false, OptionType::Float);

	DECLARE_OPTION(PRIORITY, "PRIORITY", false, OptionType::Int);

	// The option map solves a bit of an interesting.. "problem". Different builds of
	// Sly 2 may have different option indicies. However, the option data type itself
	// may not actually differ. Therefore, we use this OptionDescriptor/OptionMap structure
	// to actually map option indicies to option descriptor structs.
	// clang-format off

	BEGIN_OPTION_MAP(NtscU, Release)
		// CCm
		OPTION_MAP(161, FIELD_OF_VIEW),
		OPTION_MAP(162, FIELD_OF_VIEW_TARGET),
		OPTION_MAP(163, FIELD_OF_VIEW_SMOOTHING),
		OPTION_MAP(164, NEAR_CLIP),
		OPTION_MAP(165, FAR_CLIP),
		OPTION_MAP(166, NEAR_FOG),
		OPTION_MAP(167, FAR_FOG),
		OPTION_MAP(168, FOG_COLOR),
		OPTION_MAP(169, FOG_MAX),

		OPTION_MAP(175, PRIORITY),

		// Sw
		OPTION_MAP(913, WID),
		OPTION_MAP(914, SHADOW_HSV),
		OPTION_MAP(915, AMBIENT_MIDTONE),
		OPTION_MAP(916, AMBIENT_SHADOW),
	END_OPTION_MAP(NtscU, Release);

	// clang-format on

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
#undef X
	}
} // namespace sly::sly2::brx
