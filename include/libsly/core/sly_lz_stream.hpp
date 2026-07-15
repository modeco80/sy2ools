#pragma once
#include <mco/io/stream.hpp>

namespace sly::core {

	/// An mcolib2 adapter stream which produces a decompressed byte stream from a Sly LZ77 compressed data stream.
	///
	/// This is a fair bit more complicated than the SlyTools decompressor, owing to the fact that it is
	/// essentially intended to work (and implemented nearly) exactly the same as the original game's code.
	/// This means a lot of weird idiosyncracies the game does had to be reimplemented here, which is kind of annoying.
	///
	/// However, this also means that decompressing a compressed bitstream requires an order of magnitude less memory.
	/// (about ~24 KB over.. almost ~96 MB at worst [retail sly 2 US jb_intro.brm]?)
	class SlyLzStream : public mco::Stream {
		constexpr static u32 kBufferSize = 0x2000; // fixed compressed chunk size. doubles as window size
		constexpr static auto kBufferSizeMask = (kBufferSize - 1);

		constexpr static u32 kSpilloverSize = 0x4000;

		/// The inner stream, containing the compresed data.
		mco::Stream& inner;

		// Buffering state

		/// The read buffer.
		u8* readBuffer;

		/// Index which indicates how much we have consumed from the read buffer.
		u32 readBufferConsumed;

		/// How many bytes were produced inside of the read buffer.
		u32 readBufferProduced;

		// Decompression state

		u8* decompressBuffer;

		/// Index which indicates how much we have consumed from the decompression buffer.
		u32 decompressBufferConsumed;

		/// How many bytes were produced inside of the decompression buffer.
		u32 decompressBufferProduced;

		u32 decompressSavedLZFlags;
		u32 decompressWindowSpillover;

		/// Pumps the inner stream to read new bytes from the inner stream.
		void pumpReadBuffer();
		void decompress();
		void reset();
		u64 readFromDecompressionBuffer(u8* dstBuffer, u64 length);

	   public:
		explicit SlyLzStream(mco::Stream& inner);

		~SlyLzStream();

		bool isRandomAccess() const noexcept override;

		u64 read(void* buffer, u64 length) override;

		void align(i32 by);
	};

} // namespace sly::core
