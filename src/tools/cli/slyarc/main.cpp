#include <cstdio>

#include "cmd.hpp"

/// Usage function.
void mainUsage(char* progname) {
	std::printf(
	"slyarc - libsly archive tool\n"
	"(c) 2026 sly2ools developers under the MIT License\n"
	"Usage: %s [command] [arguments...]\n"
	"Commands:\n",
	progname);

	// Call help of each command.
	slyarc::Command::forEach([](slyarc::Command* cmd) {
		std::printf("%c - ", cmd->getCommandCharacter());
		cmd->help();
		return true;
	});
}

int main(int argc, char** argv) {
	// we need a command to run
	if(argc < 2) {
		mainUsage(argv[0]);
		return 1;
	}

	// Run the command.
	if(auto cmd = slyarc::Command::find(argv[1][0]); cmd.has_value()) {
		auto ret = (*cmd)->run(argc - 2, argv + 2);
		if(ret != 0) {
			mainUsage(argv[0]);
		}
		return ret;
	}

	printf("Error: Unknown command '%c'.\n", argv[1][0]);
	mainUsage(argv[0]);
	return 1;
}
