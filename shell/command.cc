#include <stdio.h>
#include <stdlib.h>

#include "command.h"

using namespace std;

namespace shell {

void InitDefaultCommand(Shell *sh){
	sh->Register("exit", exit);
	sh->Register("emu", emu);
}

int exit(Shell *sh, Emulator *emu, vector<string> args){
	::exit(0);
}

int emu(Shell *sh, Emulator *emu, vector<string> args){
	
}

};
