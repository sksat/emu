#include "emulator.h"

#include <iostream>

using namespace std;

vector<Emulator*> Emulator::instances;

Emulator::Emulator(){
	instances.push_back(this);
}

void Emulator::Init(int memory_size, uint32_t ip, uint32_t sp){
	bit_mode = DEFAULT_BIT_MODE;
	memory.Init(memory_size);
	IP = ip;
	SP = sp;
}

void Emulator::Start(){
	if(emu_thread != nullptr) delete emu_thread;
	emu_thread = new thread(&Emulator::emu_proc, this);
	return;
}

int Emulator::GetBitMode(){
	return bit_mode;
}

bool Emulator::IsHalt(){
	return halt_flg;
}

bool Emulator::IsReal(){
	if((CR0 & 0x80000000) == 0) return true;
	return false;
}

uint8_t Emulator::GetCode8(int index){
	return memory[pc.reg64 + index];	// 一応64bitにしておく
}

uint32_t Emulator::GetCode32(int index){
	uint32_t ret = 0;

	// little endian
	for(int i=0;i<4;i++){
		ret |= GetCode8(index + i) << (i * 8);
	}
	return ret;
}

int Emulator::emu_proc(){
	try{
//		if(emu == nullptr) throw "emu_proc: emulator is null";
//		for(int i=0;i<4096;i++)
//			cout<<hankaku[i];
		//TODO parse args
//		if(argc < 2) return -1;

//		emu = new Emulator();
//		emu->Init(DEFAULT_MEMORY_SIZE, 0x7c00, 0x7c00);

//		sh  = new Shell(emu);
//		ifstream ifs;
//		ifs.open(".emurc");
//		sh->Exec(ifs);
//		sh->ChangeStream(ifs);
//		sh->sh_proc();
//		sh->SetDefaultStream();
//		sh->Start();

//		disp = new Display();
//		disp->Init();

//		emu->memory.MapMemory(disp->vram, 0xa0000, 0xffff);
		
//		emu->LoadBinary(argv[1], 0x7c00, 512); // IPLの読み込み(本来BIOSがやるべき)
		
		while(true){
			int bit = GetBitMode();
			uint8_t code = GetCode8(0);
			
			if(IsHalt()){
				cerr<<"halt."<<endl;
				continue;
			}
		
//			cerr<<"EIP = "<<hex<<showbase<<emu->EIP;
//			cerr<<", code = "<<hex<<showbase<<(int)code<<endl;
		
			switch(bit){
			case 16:
				insn16.Exec(this, code);
				break;
			case 32:
				insn32.Exec(this, code);
				break;
			case 64:
				cout<<"64bit is not implemtented."<<endl;
				break;
			default:
				cout<<"bit mode error."<<endl;
				break;
			};
		}

		memory.Dump("memdump.bin", 0x00, 1 * MB);

	}catch(Interrupt *i){	// 割り込み処理
		cerr<<"interrupt."<<endl;
	}catch(string str){
		cerr<<"error: "<<str<<endl;
//		errExit();
		return -1;
	}catch(const char *str){
		cerr<<"error: "<<str<<endl;
//		errExit();
		return -1;
	}catch(...){
		cerr<<"exception."<<endl;
		return -1;
	}
	return 0;
}

