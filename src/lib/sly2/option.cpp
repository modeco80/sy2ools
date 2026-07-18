#include <libsly/sly2/option.hpp>

namespace sly::sly2 {

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

} // namespace sly::sly2
