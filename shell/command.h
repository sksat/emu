#ifndef SHELL_COMMAND_H_
#define SHELL_COMMAND_H_

#include "shell.h"

namespace shell {

	void InitDefaultCommand(Shell *sh);
	int alias(Shell *sh, int argc, char **argv);

};

#endif
