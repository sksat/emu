#include <stdio.h>
#include <stdlib.h>

#include "command.h"

using namespace std;

namespace shell {

void InitDefaultCommand(Shell *sh){
	sh->Register("exit", exit);
	sh->Register("test", test);
	sh->Register("print", print);
}

int exit(Shell *sh, Emulator *emu, vector<string> args){
	::exit(0);
}

int test(Shell *sh, Emulator *emu, vector<string> args){
	cout<<"test command."<<endl;
}

int print(Shell *sh, Emulator *emu, vector<string> args){
	cout<<"EIP : "<<hex<<showbase<<emu->EIP<<endl;
	cout<<"ESP : "<<hex<<emu->ESP<<endl;
}

};
