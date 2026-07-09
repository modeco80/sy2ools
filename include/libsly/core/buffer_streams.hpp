#pragma once
//! Stream adapters which buffer reads/writes to underlying streams.
//! TODO: These should go in mcolib proper.
#include <mco/io/stream.hpp>

namespace sly::core {

	/// A read-only stream adapter which buffers reads from an underlying stream.
	class BufferedReadStream : public mco::Stream {
		u32 bufferSize;
		u8* buffer;
		u32 bufferRead;
		u32 bufferWrite;

		mco::Stream& inner;

		void pumpInnerStream();
		u64 readImpl(u8* pBuffer, u64 len);

	   public:
		/// Constructor. [bufferSize] controls the buffer size
		BufferedReadStream(mco::Stream& inner, u32 bufferSize);

		BufferedReadStream(const BufferedReadStream&) = delete;
		BufferedReadStream(BufferedReadStream&&) = delete;

		~BufferedReadStream();

		bool isRandomAccess() const noexcept override;

		u64 read(void* buffer, u64 length) override;
	};

	/// A write-only stream adapter which buffers writes to an underlying stream.
	class BufferedWriteStream : public mco::WritableStream {
		u32 bufferSize;
		u8* buffer;
		u32 bufferWritePtr;

		mco::WritableStream& inner;

		u64 writeImpl(const u8* pBuffer, u64 len);

	   public:
		BufferedWriteStream(mco::WritableStream& inner, u32 bufferSize);

		BufferedWriteStream(const BufferedWriteStream&) = delete;
		BufferedWriteStream(BufferedWriteStream&&) = delete;

		~BufferedWriteStream();

		bool isRandomAccess() const noexcept override;

		u64 read(void* buffer, u64 length) override;

		u64 write(const void* buffer, usize length) override;
		/// Flushes any writes which have not been written to the underlying stream yet.
		void flush();
	};

} // namespace sly::core
