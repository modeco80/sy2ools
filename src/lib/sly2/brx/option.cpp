#include <libsly/sly2/brx/option.hpp>

namespace sly::sly2::brx {

	namespace impl {

#define X(key, _v, T, member)               \
	OptionValue::OptionValue(const T val) { \
		type = OptionType::key;             \
		member = val;                       \
	}
		LIBSLY_OPTION_TYPES()
#undef X

		OptionValue::OptionValue() {
			type = OptionType::Void;
		}

		OptionValue::OptionValue(const OptionValue& other) {
			this->type = other.type;
#define X(key, _v, T, member)         \
	if(other.type == OptionType::key) \
		this->member = other.member;
			LIBSLY_OPTION_TYPES()
#undef X
		}

	} // namespace impl

	void OptionList::addOption(const OptionDescriptor* option, const OptionValue& value) {
		// probably we should return false to indicate failure?
		if(!value.matchesType(option->type)) {
			printf("how do you not match the type? %d\n", option->type);
			return;
		}
		entries.push_back(Entry {
		.pOptionDescriptor = option,
		.value = value });
	}

	const usize OptionList::getOptionCount() const {
		return entries.size();
	}

	const OptionList::Entry* OptionList::getOption(usize index) const {
		if(index > entries.size())
			return nullptr;
		return &entries[index];
	}
} // namespace sly::sly2::brx
