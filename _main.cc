#include <iostream>
#include <string>
#include "_emulator.h"

using namespace std;

#define emu (*_emu)

int main(int argc, char **argv){

try{
	Emulator *_emu;
	_emu = new Emulator(ARCH::x86);

	emu->insn->Init();
	emu->memory = new Memory();
	emu->memory->Init(DEFAULT_MEMORY_SIZE);
	emu->memory->LoadBinary("sample/test.bin", 0x00, 512);

	cout<<"emulation start"<<endl;

	while(true){
		emu->insn->ExecStep();
	}

	cout<<"emulator deleted"<<endl;

	delete _emu;

}catch(const char *msg){
	cout<<"error:\n\t"<<msg<<endl;
}catch(string msg){
	cout<<"error:\n\t"<<msg<<endl;
}

}
