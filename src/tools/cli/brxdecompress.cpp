#include <mco/io/file_stream.hpp>

#define USE_SLY_LZ_STREAM // Use decompression adapter stream.

#ifdef USE_SLY_LZ_STREAM
	#include <libsly/core/buffer_streams.hpp>
	#include <libsly/core/sly_lz_stream.hpp>
#else
	#include <libsly/core/sly_lz.hpp>
#endif

#include <unistd.h>

/// A Mcolib2 stream which reads/writes to a standard file descriptor.
class StandardIoStream : public mco::WritableStream {
	int fd;

   public:
	enum class StandardFile {
		StandardOutput,
		StandardError
	};

   private:
	constexpr static int getStandardFd(StandardFile f) {
		switch(f) {
			case StandardFile::StandardOutput:
				return STDOUT_FILENO;
			case StandardFile::StandardError:
				return STDERR_FILENO;
			default:
				return -1;
		}
	}

   public:
	bool isRandomAccess() const noexcept override {
		return false;
	}

	explicit constexpr StandardIoStream(StandardFile file) : fd(getStandardFd(file)) {
	}

	u64 read(void* buffer, u64 n) override {
		auto nRead = ::read(fd, buffer, n);
		if(nRead == -1) {
			throw std::system_error { errno, std::generic_category() };
		}
		return nRead;
	}

	u64 write(const void* buffer, usize length) override {
		auto nWritten = ::write(fd, buffer, length);
		if(nWritten == -1) {
			throw std::system_error { errno, std::generic_category() };
		}
		return nWritten;
	}
};

// TODO: This should be in mcolib2 proper.
/// Tees two streams until [src] does not have any bytes left.
void teeStreamsFull(mco::Stream& src, mco::WritableStream& dst) {
	u8 readBuffer[2048] {};
	while(true) {
		const auto nRead = src.read(&readBuffer[0], sizeof(readBuffer));
		if(nRead == 0)
			break;
		if(dst.write(&readBuffer[0], nRead) != nRead)
			break;
	}
}

int main(int argc, char** argv) {
	if(argc != 2) {
		printf(
		"Usage: %s [compressed BRX file]\n"
		"Writes decompressed content to standard output.\n",
		argv[0]);
		return 1;
	}

	auto compressedFile = mco::FileStream::open(argv[1], mco::FileStream::Read);
	auto dstStream = StandardIoStream(StandardIoStream::StandardFile::StandardOutput);

#ifdef USE_SLY_LZ_STREAM
	auto bufferDst = sly::core::BufferedWriteStream(dstStream, 0x2000);
	auto decompressStream = sly::core::SlyLzStream(compressedFile);
	teeStreamsFull(decompressStream, bufferDst);
#else
	sly::core::slylz77Decompress(compressedFile, dstStream);
#endif

	return 0;
}
