#pragma once
#include <libsly/sly2/brx/option.hpp>
#include <mco/base_types.hpp>

namespace sly::sly2::brx {
	class Parser;

	/// An object in the world.
	struct Object {
		std::string name;
		OptionList options;

		// Parse into this object. TODO
		//virtual bool parse(Parser& parser);
	};

} // namespace sly::sly2::brx
