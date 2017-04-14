#include <iostream>
#include <string>
#include "emulator.h"
#include "device/display.h"
#include "shell/shell.h"

using namespace std;

Emulator *emu;
Display  *disp;
Shell    *sh;
//extern char hankaku[4096];

void errExit(){
	if(emu == nullptr) exit(-1);
	emu->memory.Dump("memdump.bin", 0x00, 1 * MB);
	exit(-1);
}

int main(int argc, char **argv){
	try{
//		for(int i=0;i<4096;i++)
//			cout<<hankaku[i];
		//TODO parse args
		if(argc < 2) return -1;

		emu = new Emulator();
		emu->Init(DEFAULT_MEMORY_SIZE, 0x7c00, 0x7c00);

		sh  = new Shell(emu);
		sh->Start();

		disp = new Display();
		disp->Init();

		emu->memory.MapMemory(disp->vram, 0xa0000, 0xffff);
		
		emu->LoadBinary(argv[1], 0x7c00, 512); // IPLの読み込み(本来BIOSがやるべき)
		
		while(true){
			int bit = emu->GetBitMode();
			uint8_t code = emu->GetCode8(0);
			
			if(emu->IsHalt()){
				cout<<"halt."<<endl;
				continue;
			}
		
//			cout<<"EIP = "<<hex<<showbase<<emu->EIP;
//			cout<<", code = "<<hex<<showbase<<(int)code<<endl;
		
			switch(bit){
			case 16:
				emu->insn16.Exec(emu, code);
				break;
			case 32:
				emu->insn32.Exec(emu, code);
				break;
			case 64:
				cout<<"64bit is not implemtented."<<endl;
				break;
			default:
				cout<<"bit mode error."<<endl;
				break;
			};
		}

		emu->memory.Dump("memdump.bin", 0x00, 1 * MB);

	}catch(Interrupt *i){	// 割り込み処理
		cerr<<"interrupt."<<endl;
	}catch(string str){
		cerr<<"error: "<<str<<endl;
		errExit();
	}catch(const char *str){
		cerr<<"error: "<<str<<endl;
		errExit();
	}catch(...){
		cerr<<"exception."<<endl;
		return -1;
	}
	return 0;
}


