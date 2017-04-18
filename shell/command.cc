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
	int i=0, size = args.size();
	if(size > 2){
		cout<<"exit: Too many arguments."<<endl;
		return -1;
	}

	if(size == 1) goto exit;
	sscanf(args[1].c_str(), "%d", &i);
exit:
	::exit(i);
}

int test(Shell *sh, Emulator *emu, vector<string> args){
	cout<<"test command."<<endl;
}

int print(Shell *sh, Emulator *emu, vector<string> args){
	cout<<"EIP : "<<hex<<showbase<<emu->EIP<<endl;
	cout<<"ESP : "<<hex<<emu->ESP<<endl;
}

};
