#include <libsly/sly2/brx/parser.hpp>
#include <libsly/sly2/iso_filesystem.hpp>

void dumpOptions(sly::sly2::brx::Object& object) {
	printf("Object %s options:\n", object.name.c_str());
	for(auto i = 0; i < object.options.getOptionCount(); ++i) {
		auto* option = object.options.getOption(i);

		std::printf("Option %s, ", option->pOptionDescriptor->pszName);
		switch(option->pOptionDescriptor->type) {
			case sly::sly2::brx::OptionType::Float:
				printf("Float %f\n", option->value.get<float>());
				break;

			case sly::sly2::brx::OptionType::Vector: {
				const auto& v = option->value.get<sly::sly2::Vector>();
				printf("Vector (%f,%f,%f)\n", v.x, v.y, v.z);
			} break;

			case sly::sly2::brx::OptionType::Smpa: {
				const auto& v = option->value.get<sly::sly2::brx::SmoothingParameters>();
				printf("Smoothing Parameters (%f,%f,%f,%f)\n", v.svFast,v.svSlow,v.dtFast,v.sfvMax);
			} break;

			case sly::sly2::brx::OptionType::Rgba: {
				const auto& v = option->value.get<sly::sly2::brx::Rgba>();
				printf("RGB Color (%d,%d,%d,%d)\n", v.r,v.g,v.b,v.a);
			} break;

			case sly::sly2::brx::OptionType::Wid: {
				const auto& v = option->value.get<sly::sly2::brx::Wid>();
				printf("WorldId (%d)\n", v);
			} break;

			default:
				break;
		}
	}
}

int main(int argc, char** argv) {
	auto fs = mco::FileStream::open(argv[1], mco::FileStream::Read);
	auto isofs = sly::sly2::IsoFileSystem(std::move(fs));

	sly::sly2::brx::BrxData data;
	sly::sly2::brx::Object worldObject;

	// Load the intro BRX
	auto parser = sly::sly2::brx::Parser(isofs, sly::sly2::FileLocation("FK$Zjb_intro"));

	if(!parser.parseAll(data)) {
		printf("failed to parse brx\n");
		return 1;
	}

#if 0

	for(auto& proxyent : data.proxyTable) {
		printf("%32s %d %d %d %d %d\n", proxyent.name, proxyent.unk1, proxyent.unk2, proxyent.oid, proxyent.unk4, proxyent.unk5);
	}

	for(auto& voent : data.sound.voiceLines) {
		auto tmp = voent.languages[0].vagLocation.toString();
		printf("voiceline = %s runtime %f\n", tmp.c_str(), voent.languages[0].flRuntime);
	}

	// for(auto& ent : data.save.hashTableEntries) {
	//	printf("save hash %s = %d\n", ent.key.c_str(), ent.value);
	// }

	for(auto& ent : data.worldTable.worlds) {
		auto tmp = ent.locBrx.toString();
		printf("WorldTable %s\n", tmp.c_str());
	}

	for(auto& str : data.text.stringTable) {
		printf("string id %04x = %s\n", str.stringId, str.strings[0].c_str());
	}
#endif

	dumpOptions(*data.worldObject);
	dumpOptions(*data.cameraObject);

	printf("parsed it!\n");
	return 0;
}
