#include <libsly/core/buffer_streams.hpp>
#include <libsly/sly2/iso_filesystem.hpp>
#include <libsly/sly2/release.hpp>
#include <mco/io/file_stream.hpp>
#include <mco/io/stream_utils.hpp>

int main(int argc, char** argv) {
	if(argc != 2) {
		printf("Usage: %s [path to Sly 2 ISO]\n", argv[0]);
		return 1;
	}

	auto fs = mco::FileStream::open(argv[1], mco::FileStream::Read);
	auto isofs = sly::sly2::IsoFileSystem(std::move(fs));

	auto rel = isofs.getRelease();
	if(rel == sly::sly2::Release::Invalid) {
		printf("Unsupported ISO, or not Sly 2.\n");
		return 1;
	}

	printf("Detected %s\n", sly::sly2::getReleaseString(rel).c_str());

	isofs.enumFilesLambda([](const char* pszFileName, u32 size) {
		printf("%s %d\n", pszFileName, size);
		return true;
	});

	return 0;
}
