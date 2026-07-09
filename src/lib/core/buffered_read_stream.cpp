#include <cstring>
#include <libsly/core/buffer_streams.hpp>

namespace sly::core {

	void BufferedReadStream::pumpInnerStream() {
		auto n = inner.read(&buffer[0], bufferSize);
		bufferRead = 0;
		bufferWrite = n;
	}

	u64 BufferedReadStream::readImpl(u8* pBuffer, u64 len) {
		if(bufferWrite == 0)
			return 0;

		u64 counter = len;

		while(counter) {
			const auto n = std::min<u64>(len, bufferWrite - bufferRead);
			std::memcpy(&pBuffer[(len - counter)], &buffer[bufferRead], n);
			bufferRead += n;
			counter -= n;

			if(bufferRead >= bufferWrite) {
				// Out of buffer space. Try to fill the buffer from the underlying stream.
				// If there is no more data left, simply give up.
				pumpInnerStream();
				if(bufferWrite == 0)
					break;
			}
		}

		return len - counter;
	}

	BufferedReadStream::BufferedReadStream(mco::Stream& inner, u32 bufferSize)
		: inner(inner), bufferSize(bufferSize) {
		buffer = new u8[bufferSize];

		// do an initial pump.
		pumpInnerStream();
	}

	BufferedReadStream::~BufferedReadStream() {
		delete[] buffer;
	}

	bool BufferedReadStream::isRandomAccess() const noexcept {
		return false;
	}

	u64 BufferedReadStream::read(void* buffer, u64 length) {
		return readImpl(static_cast<u8*>(buffer), length);
	}

} // namespace sly::core
