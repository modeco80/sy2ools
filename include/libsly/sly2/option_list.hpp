#pragma once
#include <libsly/sly2/option.hpp>

namespace sly::sly2 {

	/// Holds a list of options and their data.
	class OptionList {
	   public:
		struct Entry {
			const OptionDescriptor* pOptionDescriptor;
			OptionValue value;
		};

	   private:
		std::vector<Entry> entries;

	   public:
		void addOption(const OptionDescriptor* option, const OptionValue& value);

		// TODO for editing/cleaner support
		// - removeOption(u32 index);
		// - enumerateOptions() (enumerates all options on the object)

		const usize getOptionCount() const;

		/// Gets an option at the provided index.
		const Entry* getOption(usize index) const;
	};

} // namespace sly::sly2
