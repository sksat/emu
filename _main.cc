#include <iostream>
#include <string>
#include "_emulator.h"

using namespace std;

#define emu (*_emu)

int main(int argc, char **argv){

try{
	Emulator *_emu;
	_emu = new Emulator(ARCH::osecpu);

	emu->memory->Init(0xffff);
	emu->memory->LoadBinary("sample/osecpu/app0100.ose", 0x00, 512);

	cout<<"emulation start"<<endl;
int i=0x00;
	while(true){
		cout<<hex<<(uint32_t)emu->memory[i];i++;
	}

}catch(const char *msg){
	cout<<"error:\n\t"<<msg<<endl;
}catch(string msg){
	cout<<"error:\n\t"<<msg<<endl;
}

}
