#include <iostream>
#include <string>
#include "emulator.h"

using namespace std;

Emulator *emu;

int main(int argc, char **argv){
	try{
		//TODO parse args
		if(argc < 2) return -1;
		
		emu = new Emulator();
		emu->Init(DEFAULT_MEMORY_SIZE);
		
		emu->LoadBinary(argv[1], 0x7c00, 512); // IPLの読み込み(本来BIOSがやるべき)
		
		while(true){
			int bit = emu->GetBitMode();
			uint8_t code = emu->GetCode8(0);
			
			if(emu->IsHalt()){
				cout<<"halt."<<endl;
				continue;
			}
			
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
		
	}catch(Interrupt *i){	// 割り込み処理
		cout<<"interrupt."<<endl;
	}catch(string str){
		cout<<"error: "<<str<<endl;
		return -1;
	}catch(const char *str){
		cout<<"error: "<<str<<endl;
		return -1;
	}catch(...){
		cout<<"exception."<<endl;
		return -1;
	}
	return 0;
}


