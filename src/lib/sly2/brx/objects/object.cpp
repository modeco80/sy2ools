#include <libsly/sly2/brx/objects/object.hpp>
#include <libsly/sly2/brx/parser.hpp>

namespace sly::sly2::brx {

	bool Object::parse(Parser& parser) {
		return parser.parseOptions(options);
	}

}
