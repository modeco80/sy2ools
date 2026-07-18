#include <libsly/sly2/option_list.hpp>

namespace sly::sly2 {
	void OptionList::addOption(const OptionDescriptor* option, const OptionValue& value) {
		// probably we should return false to indicate failure?
		if(!value.matchesType(option->type)) {
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
} // namespace sly::sly2
