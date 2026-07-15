#pragma once
#include <libsly/sly2/brx/option.hpp>
#include <libsly/sly2/release.hpp>
#include <mco/base_types.hpp>

namespace sly::sly2::brx {

#define OPTION_REFERENCE(optName) \
	sOption##optName

	// declare an option
#define DECLARE_OPTION(optName, optionName, _bSplice, optType)            \
	constinit const static OptionDescriptor OPTION_REFERENCE(optName) = { \
		.pszName = optionName,                                            \
		.bSpliceCall = _bSplice,                                          \
		.type = optType                                                   \
	}

#define BEGIN_OPTION_MAP(region, version) \
	constexpr static OptionMap::Entry sOptionMapEntries_##region##_##version[] = {
#define OPTION_MAP(_optionId, _optName) \
	{ .optionId = _optionId, .option = &OPTION_REFERENCE(_optName) }

#define END_OPTION_MAP(region, version)                                                        \
	}                                                                                          \
	;                                                                                          \
	const static OptionMap sOptionMap_##region##_##version = {                                 \
		.nOptions = sizeof(sOptionMapEntries_##region##_##version) / sizeof(OptionMap::Entry), \
		.pOptions = &sOptionMapEntries_##region##_##version[0]                                 \
	}

	struct OptionMap {
		struct Entry {
			i32 optionId;
			const OptionDescriptor* option;
		};

		usize nOptions;
		const Entry* pOptions;

		const OptionDescriptor* find(i16 opid) const;
		const OptionDescriptor* find(const char* pszName) const;
	};

	const OptionMap* getOptionMapForRelease(Release release);

} // namespace sly::sly2::brx
