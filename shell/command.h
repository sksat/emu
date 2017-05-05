#ifndef SHELL_COMMAND_H_
#define SHELL_COMMAND_H_

#include "shell.h"

namespace shell {

	void InitDefaultCommand(Shell *sh);
	int alias(Shell *sh, std::vector<std::string> arg);
	int exit(Shell *sh, std::vector<std::string> args);
	int test(Shell *sh, std::vector<std::string> args);
	int print(Shell *sh, std::vector<std::string> args);
	int init(Shell *sh, std::vector<std::string> args);
	int load(Shell *sh, std::vector<std::string> args);
	int run(Shell *sh, std::vector<std::string> args);
	int startg(Shell *sh,  std::vector<std::string> args);
	int congui(Shell *sh, std::vector<std::string> args);

};

#endif
