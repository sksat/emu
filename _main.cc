#include <iostream>
#include <string>
#include <chrono>
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
//	emu->reg[4].reg32 = 0x7c00;
//	emu->reg[8].reg32 = 0x7c00;

	cout<<"emulation start"<<endl;

	auto start = std::chrono::system_clock::now();

	while(!emu->finish_flg){
		emu->insn->ExecStep();
	}

	auto end = std::chrono::system_clock::now();

	emu->Dump();

	std::cout<<"time: "<<(double)std::chrono::duration_cast<std::chrono::seconds>(end - start).count()<<"s"<<std::endl;

	cout<<"emulator deleted"<<endl;
	delete _emu;

}catch(const char *msg){
	cout<<endl<<"error:\n\t"<<msg<<endl;
	emu->Dump();
	delete _emu;
}catch(string msg){
	cout<<endl<<"error:\n\t"<<msg<<endl;
	emu->Dump();
	delete _emu;
}

}
