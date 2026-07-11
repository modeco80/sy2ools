#include <libsly/sly2/iso_filesystem.hpp>
#include <libsly/sly2/brx/parser.hpp>
#include "libsly/sly2/brx/data.hpp"

int main(int argc, char** argv) {
	auto fs = mco::FileStream::open(argv[1], mco::FileStream::Read);
	auto isofs = sly::sly2::IsoFileSystem(std::move(fs));

	sly::sly2::brx::BrxData data;
	auto parser = sly::sly2::brx::Parser(isofs, "intro.brx");

	if(!parser.parseAll(data)) {
		printf("failed to parse brx\n");
	}

	for(auto& proxyent : data.proxyTable) {
		printf("%32s %d %d %d %d %d\n", proxyent.name, proxyent.unk1, proxyent.unk2, proxyent.oid, proxyent.unk4, proxyent.unk5);
	}

	printf("parsed it!\n");
	return 0;

}
