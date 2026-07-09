#include <libsly/core/buffer_streams.hpp>
#include <libsly/core/sly_lz.hpp>

namespace sly::core {

	constexpr static u32 kLZWindowSize = 0x2000;
	constexpr static u32 kLZThreshold = 3;

	void slylz77Decompress(mco::Stream& src, mco::WritableStream& dst) {
		auto srcBuffered = BufferedReadStream(src, kLZWindowSize);
		auto dstBuffered = BufferedWriteStream(dst, kLZWindowSize);

// These macros aren't strictly required, but they reduce the complication of the
// implementation and help keep some logic from being overtly obscured by boring
// stream stuff.
#define lzGetByte(c)                                 \
	u8 c;                                            \
	if(srcBuffered.read(&c, sizeof(c)) != sizeof(c)) \
		break;
#define lzPutByte(c)                                  \
	if(dstBuffered.write(&c, sizeof(c)) != sizeof(c)) \
		break;                                        \
	lzWindow[windowIndex++ & (kLZWindowSize - 1)] = c;

		u32 windowIndex = 0;
		u32 lzFlags = 0;
		u8 lzWindow[0x2000] {};

		while(true) {
			// Have flags have been consumed?
			// If so, get new flags from the compressed bitstream.
			if(((lzFlags >>= 1) & 256) == 0) {
				lzGetByte(newFlags);
				lzFlags = newFlags | 0xff00;
			}

			// Read the first command byte.
			//
			// Based on the flags, this can either be:
			// - the first byte of a match command
			// - an uncompressed literal byte.
			lzGetByte(cmd1);

			if(lzFlags & 1) {
				// A literal byte.
				lzPutByte(cmd1);
			} else {
				// A match. Read the second command byte emitted for the match,
				// and combine it into a single command word.
				lzGetByte(cmd2);
				const u16 command = cmd1 | (cmd2 << 8);

				// Once the command has been combined, split it back into the match offset and length,
				// and finally decompress the match.
				u32 matchOffset = command & (kLZWindowSize - 1);
				u32 matchLength = (command >> 0xd) + kLZThreshold;
				while(matchLength--) {
					lzPutByte(lzWindow[matchOffset]);
					matchOffset = (matchOffset + 1) & (kLZWindowSize - 1);
				}
			}
		}

#undef lzGetByte
#undef lzPutByte
	}

} // namespace sly::core
