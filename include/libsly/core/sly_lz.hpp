#pragma once
#include <mco/io/stream.hpp>

namespace sly::core {

	/// Performs sly-lz77 decompression on the input stream [src], writing the decompressed
	/// data to the output stream [dst].
	void slylz77Decompress(mco::Stream& src, mco::WritableStream& dst);

} // namespace sly::core
