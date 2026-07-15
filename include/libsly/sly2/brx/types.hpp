#pragma once
#include <mco/base_types.hpp>

namespace sly::sly2::brx {

	enum class Wid : i16 {};

	struct Rgba {
		u8 r;
		u8 g;
		u8 b;
		u8 a;
	};

	struct SmoothingParameters {
		float svFast;
		float svSlow;
		float dtFast;
		float sfvMax;
	};

	mcoAssertSize(Rgba, 4);
	mcoAssertSize(SmoothingParameters, 16);

}
