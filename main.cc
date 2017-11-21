#include <iostream>
#include <string>
#include <chrono>
#include <sksat/cmdline.hpp> // https://github.com/sk2sat/libsksat
#include "emulator.h"

EmulatorCtrl::Setting set;
EmulatorCtrl emuctrl;

int main(int argc, char **argv){
using std::cout;
using std::endl;
using std::string;
try{
	set.arch = ARCH::x86;
	set.memsize = 1; // 1MB

	sksat::optparse o;
	string arch_str;
	string fda_file; // A drive (floppy)

	o.add_opt(arch_str, 'a', "arch", "architecture");
	o.add_opt(set.junk_bios, "junk-bios", "enable junk BIOS");
	o.add_opt(set.memsize, 'm', "memory-size", "memory size(MB)");
	o.add_opt(fda_file, "fda", "floppy disk image file");

	if(!o.parse(argc, argv)){
		cout	<<"simple x86 emulator by sksat"<<endl
			<<"repo:   https://github.com/sk2sat/emu"<<endl
			<<"commit: " << GIT_COMMIT_ID <<endl
			<<"date:   " << GIT_COMMIT_DATE <<endl<<endl;
		o.print_help();
		return -1;
	}

	if(arch_str == "x86"){
		set.arch = ARCH::x86;
	}else if(arch_str == "osecpu"){
		set.arch = ARCH::osecpu;
	}else if(arch_str.empty()){
		set.arch = ARCH::x86;
	}else{
		throw "unknown arch: "+arch_str;
	}

	emuctrl.Init(set);

	auto emu = emuctrl.GetRaw();

	emu->memory->Init(set.memsize * MB);
//	emu->memory->LoadBinary("sample/harib27f.img", 0x7c00, 512);

	cout<<"memory size: "<<set.memsize<<"MB"<<endl;

	if(fda_file.empty())
		throw "no boot device.";
	Device::Floppy fda(fda_file.c_str());
	emu->ConnectDevice(fda);

	if(set.junk_bios){
		cout<<"setup junk BIOS..."<<endl;
		switch(set.arch){
		case ARCH::x86:
//			auto e = new BIOS::Junk::x86(emu.GetRaw());
			emu->SetBios(new BIOS::Junk::x86(emu));
			break;
		default:
			throw "not implemented junk BIOS for this arch.";
			break;
		}
		emu->bios->Boot();
	}

	cout<<"emulation start"<<endl;

	auto start = std::chrono::system_clock::now();

	while(!emu->finish_flg){
		emu->insn->ExecStep();
	}

	auto end = std::chrono::system_clock::now();

	emu->Dump();

	std::cout<<"time: "<<(double)std::chrono::duration_cast<std::chrono::seconds>(end - start).count()<<"s"<<std::endl;

	cout<<"emulator deleted"<<endl;
}catch(const char *msg){
	cout<<endl<<"error:\n\t"<<msg<<endl;
	emuctrl.GetRaw()->Dump();
}catch(string msg){
	cout<<endl<<"error:\n\t"<<msg<<endl;
	emuctrl.GetRaw()->Dump();
}

}
