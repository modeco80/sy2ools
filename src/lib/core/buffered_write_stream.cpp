#include <cstring>
#include <libsly/core/buffer_streams.hpp>

namespace sly::core {

	u64 BufferedWriteStream::writeImpl(const u8* pBuffer, u64 len) {
		u64 counter = len;

		while(counter) {
			auto n = std::min<u64>(len, bufferSize - bufferWritePtr);
			std::memcpy(&buffer[bufferWritePtr], &pBuffer[(len - counter)], n);
			bufferWritePtr += n;
			counter -= n;

			// The buffer is full, flush it.
			if(bufferWritePtr >= bufferSize)
				flush();
		}

		return len - counter;
	}

	BufferedWriteStream::BufferedWriteStream(mco::WritableStream& inner, u32 bufferSize)
		: inner(inner), bufferSize(bufferSize) {
		buffer = new u8[bufferSize];
		bufferWritePtr = 0;
	}

	BufferedWriteStream::~BufferedWriteStream() {
		// Flush any pending buffered data before we destroy the buffer.
		flush();
		delete[] buffer;
	}

	bool BufferedWriteStream::isRandomAccess() const noexcept {
		return false;
	}

	u64 BufferedWriteStream::read(void* buffer, u64 length) {
		return 0; // Stream adapter is not readable.
	}

	u64 BufferedWriteStream::write(const void* buffer, usize length) {
		return writeImpl(static_cast<const u8*>(buffer), length);
	}

	void BufferedWriteStream::flush() {
		if(bufferWritePtr != 0) {
			inner.write(&buffer[0], bufferWritePtr);
			bufferWritePtr = 0;
		}
	}

} // namespace sly::core
