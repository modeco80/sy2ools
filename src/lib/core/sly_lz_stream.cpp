#include <cstring>
#include <libsly/core/sly_lz_stream.hpp>

namespace sly::core {

	void SlyLzStream::pumpReadBuffer() {
		readBufferProduced = inner.read(&readBuffer[0], kBufferSize);
		readBufferConsumed = 0;
	}

	void SlyLzStream::decompress() {
		auto cbWritten = 0;
		auto flags = decompressSavedLZFlags;

#define lzGetByte(c)                               \
	if(readBufferConsumed == readBufferProduced) { \
		pumpReadBuffer();                          \
		if(readBufferProduced == 0)                \
			break;                                 \
	}                                              \
	const auto c = readBuffer[readBufferConsumed++];

		// Copy spillover into the window/decompress buffer.
		if(decompressWindowSpillover > 0) {
			std::memcpy(&decompressBuffer[0], &decompressBuffer[kBufferSize], decompressWindowSpillover);
			cbWritten = decompressWindowSpillover;
		}

		while(cbWritten < kBufferSize) {
			// Have flags have been consumed?
			// If so, get new flags from the compressed bitstream.
			if(((flags >>= 1) & 256) == 0) {
				lzGetByte(newFlags);
				// printf("DEBUG: new flags %08b\n", newFlags);
				flags = newFlags | 0xff00;
			}

			// Read the first command byte.
			//
			// Based on the flags, this can either be:
			// - the first byte of a match command
			// - a uncompressed literal byte.
			lzGetByte(cmd1);

			if(flags & 1) {
				// Just a boring literal byte.
				decompressBuffer[cbWritten++] = cmd1;
			} else {
				// A match. Read the second command byte emitted for the match,
				// and combine it into a single command word.
				lzGetByte(cmd2);
				const u16 command = cmd1 | (cmd2 << 8);

				// Once the command has been combined, split it into match offset and length,
				// and decompress the match.
				u32 matchOffset = command & kBufferSizeMask;
				u32 matchLength = (command >> 0xd) + 2;

				do {
					// Write the match.
					decompressBuffer[cbWritten++] = decompressBuffer[matchOffset];
					matchOffset = (matchOffset + 1) & kBufferSizeMask;
				} while(matchLength--);
			}
		}

		decompressSavedLZFlags = flags;

		if(cbWritten <= kBufferSize) {
			decompressBufferProduced = cbWritten;
			decompressWindowSpillover = 0;
		} else {
			decompressBufferProduced = kBufferSize;
			decompressWindowSpillover = cbWritten - kBufferSize;
		}

#undef lzGetByte
	}

	void SlyLzStream::reset() {
		// Clear all the work buffer memory in one go.
		std::memset(&readBuffer[0], 0, kBufferSize + kSpilloverSize);

		// Reset internal buffer state.
		readBufferProduced = 0;
		readBufferConsumed = 0;

		// Reset decompression state.
		decompressSavedLZFlags = 0;
		decompressWindowSpillover = 0;
		decompressBufferProduced = 0;
		decompressBufferConsumed = 0;
	}

	u64 SlyLzStream::readFromDecompressionBuffer(u8* dstBuffer, u64 length) {
		u64 i = 0;
		for(; i < length; ++i) {
			// Once we have produced all bytes from the decompression buffer, pump to read and
			// decompress more compressed data from the stream.
			// Otherwise, if we have run out of data, give up.
			if(decompressBufferConsumed == decompressBufferProduced) {
				// Decompress new data.
				decompress();
				decompressBufferConsumed = 0;

				// Decompression has finished.
				if(decompressBufferProduced == 0) {
					return i;
				}
			}

			dstBuffer[i] = decompressBuffer[decompressBufferConsumed++];
		}

		return i;
	}

	SlyLzStream::SlyLzStream(mco::Stream& inner)
		: inner(inner) {
		// Allocate work buffers.
		readBuffer = new u8[kBufferSize + kSpilloverSize];
		decompressBuffer = readBuffer + kBufferSize;

		// Reset all state, and perform an initial pump of data.
		reset();
		decompress();
	}

	SlyLzStream::~SlyLzStream() {
		// We only need to delete the read buffer, as we make all the work buffer
		// allocations in one arena, as a part of it.
		delete[] readBuffer;
		decompressBuffer = nullptr;
	}

	bool SlyLzStream::isRandomAccess() const noexcept {
		return false;
	}

	u64 SlyLzStream::read(void* buffer, u64 length) {
		return readFromDecompressionBuffer(static_cast<u8*>(buffer), length);
	}

	void SlyLzStream::align(i32 by) {
		decompressBufferConsumed = (decompressBufferConsumed + (by - 1) & -by);
		// probably should throw
		if(decompressBufferConsumed > decompressBufferProduced)
			decompressBufferConsumed = decompressBufferProduced;
	}

} // namespace sly::core
