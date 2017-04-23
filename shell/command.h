#ifndef SHELL_COMMAND_H_
#define SHELL_COMMAND_H_

#include "shell.h"

namespace shell {

	void InitDefaultCommand(Shell *sh);
	int alias(Shell *sh, Emulator *emu, std::vector<std::string> arg);
	int exit(Shell *sh, Emulator *emu, std::vector<std::string> args);
	int test(Shell *sh, Emulator *emu, std::vector<std::string> args);
	int print(Shell *sh, Emulator *emu, std::vector<std::string> args);
	int load(Shell *sh, Emulator *emu, std::vector<std::string> args);

};

#endif
