#include <stdio.h>
#include <stdlib.h>

#include "command.h"

using namespace std;

namespace shell {

void InitDefaultCommand(Shell *sh){
	sh->Register("exit", exit);
	sh->Register("test", test);
	sh->Register("print", print);
	sh->Register("load", load);
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
	if(emu == nullptr) throw "emulator is null";
	cout<<"EIP : "<<hex<<showbase<<emu->EIP<<endl;
	cout<<"ESP : "<<hex<<emu->ESP<<endl;
}

int load(Shell *sh, Emulator *emu, vector<string> args){
	if(args.size() != 3) throw "option error.";
	if(args[1] == "binary") cout<<"loading binary..."<<endl;
	cout<<"loading \""<<args[2]<<"\"."<<endl;
	if(emu == nullptr) throw "emulator is null";
	cout<<"setting default addr: 0x7c00~"<<endl;
	emu->LoadBinary(args[2].c_str(), 0x7c00, 512);
}

};
