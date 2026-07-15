#include <libsly/sly2/brx/option.hpp>
#include <cstring>

namespace sly::sly2::brx {

	namespace impl {

#define X(key, _v, T, member) \
		OptionValue::OptionValue(const T val) { \
			type = OptionType::key; \
			member = val; \
		}
		LIBSLY_OPTION_TYPES()
#undef X

		OptionValue::OptionValue() {
			std::memset(this, 0, sizeof(*this));
		}

		OptionValue::OptionValue(const OptionValue& other) {
#define X(key, _v, T, member) \
			if(other.type == OptionType::key) \
				this->member = other.member;
			LIBSLY_OPTION_TYPES()
#undef X
		}

	}

	void OptionList::addOption(const OptionDescriptor* option, const OptionValue& value) {
		// probably we should return false to indicate failure?
		if(!value.matchesType(option->type))
			return;
	}

	const OptionList::Entry* OptionList::getOption(usize index) const {
		if(index > entries.size())
			return nullptr;
		return &entries[index];
	}
} // namespace sly::sly2::brx
