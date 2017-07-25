#include <iostream>
#include <string>
#include "_emulator.h"

using namespace std;

#define emu (*_emu)

Emulator *_emu;

int main(int argc, char **argv){

try{
	_emu = new Emulator(ARCH::x86);

	emu->insn->Init();
	emu->memory->Init(DEFAULT_MEMORY_SIZE);
	emu->memory->LoadBinary("sample/test.bin", 0x7c00, 512);

	//temporary
	emu->reg[4].reg32 = 0x7c00;
	emu->reg[8].reg32 = 0x7c00;

	cout<<"emulation start"<<endl;

	while(!emu->finish_flg){
		emu->insn->ExecStep();
	}

	emu->Dump();

	cout<<"emulator deleted"<<endl;
	delete _emu;

}catch(const char *msg){
	cout<<"error:\n\t"<<msg<<endl;
	emu->Dump();
	delete _emu;
}catch(string msg){
	cout<<"error:\n\t"<<msg<<endl;
	emu->Dump();
	delete _emu;
}

}
