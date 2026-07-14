#include <libsly/sly2/brx/parser.hpp>
#include <libsly/sly2/iso_filesystem.hpp>

#include "libsly/sly2/brx/data.hpp"

int main(int argc, char** argv) {
	auto fs = mco::FileStream::open(argv[1], mco::FileStream::Read);
	auto isofs = sly::sly2::IsoFileSystem(std::move(fs));

	sly::sly2::brx::BrxData data;

	// Load the intro BRX
	auto parser = sly::sly2::brx::Parser(isofs, "FK$Zjb_intro");

	if(!parser.parseAll(data)) {
		printf("failed to parse brx\n");
	}

	for(auto& proxyent : data.proxyTable) {
		printf("%32s %d %d %d %d %d\n", proxyent.name, proxyent.unk1, proxyent.unk2, proxyent.oid, proxyent.unk4, proxyent.unk5);
	}

	for(auto& voent : data.sound.voiceLines) {
		auto tmp = voent.lineFiles[0].vagLocation.toString();
		printf("voiceline = %s\n", tmp.c_str());
	}

	for(auto& ent : data.save.hashTableEntries) {
		printf("save hash %s = %d\n", ent.key.c_str(), ent.value);
	}


	printf("parsed it!\n");
	return 0;
}
