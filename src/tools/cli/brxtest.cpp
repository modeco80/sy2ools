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
		auto tmp = voent.languages[0].vagLocation.toString();
		printf("voiceline = %s runtime %f\n", tmp.c_str(), voent.languages[0].flRuntime);
	}

	//for(auto& ent : data.save.hashTableEntries) {
	//	printf("save hash %s = %d\n", ent.key.c_str(), ent.value);
	//}

	for(auto& ent : data.worldTable.worlds) {
		auto tmp = ent.locBrx.toString();
		printf("WorldTable %s\n", tmp.c_str());
	}

	printf("parsed it!\n");
	return 0;
}
