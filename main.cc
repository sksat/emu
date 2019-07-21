#include <iostream>
#include <string>
#include <chrono>
#include "sksat/cmdline.hpp" // https://github.com/sk2sat/libsksat
#include "emulator.h"
#include "gui.h"

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
	string font_file = "font/hankaku.bin";

	{
		string exe_dir = argv[0];
		auto end = exe_dir.size();
		for(;;end--){
			if(exe_dir[end] == '/') break;
		}
		auto path = exe_dir.substr(0, end);
		font_file = path + '/' + font_file;
	}

	o.add_opt(arch_str, 'a', "arch", "architecture");
	o.add_opt(set.junk_bios, "junk-bios", "enable junk BIOS");
	o.add_opt(set.memsize, 'm', "memory-size", "memory size(MB)");
	o.add_opt(set.gui, "gui", "with GUI");
	o.add_opt(fda_file, "fda", "floppy disk image file");
	o.add_opt(font_file, "font", "font file");

	// settings
	arch_str = "x86";
	set.junk_bios = true;
	set.memsize = 4;
	set.gui = true;
	fda_file = "sample/haribote.img";

/*
	if(!o.parse(argc, argv)){
		cout	<<"simple x86 emulator by sksat"<<endl
			<<"repo:   https://github.com/sk2sat/emu"<<endl
			<<"commit: " << GIT_COMMIT_ID <<endl
			<<"date:   " << GIT_COMMIT_DATE <<endl<<endl;
		o.print_help();
		return -1;
	}
*/

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

	Device::Floppy fda;
	if(!fda_file.empty()){
		fda.Open(fda_file);
		emu->ConnectDevice(fda);
	}

	std::unique_ptr<Gui> gui;
	Device::Display disp;
	auto& port = emu->io->port;
	port[0x03c8] = port[0x03c9] = &disp;
	bool halt_exit = false;
	if(set.gui){
		disp.LoadFont(font_file);
		disp.RegisterVRAM(emu->memory, 0xa0000, 0xffff);
		emu->ConnectDevice(disp);

		gui = std::make_unique<Gui>();
		gui->onExit = [&](){ emu->finish_flg = true; };
		gui->Start(disp);
	}else{
		halt_exit = true; // CLIだったらhaltした時に終了するようにする
	}

	if(set.junk_bios){
		cout<<"setup junk BIOS..."<<endl;
		switch(set.arch){
		case ARCH::x86:
//			auto e = new BIOS::Junk::x86(emu.GetRaw());
			emu->SetBios(new BIOS::Junk::x86(dynamic_cast<x86::Emulator*>(emu)));
			break;
		default:
			throw "not implemented junk BIOS for this arch.";
			break;
		}
		if(set.gui) emu->bios->SetDisplay(disp);
		emu->bios->Boot();
	}

	cout<<"emulation start"<<endl;

	auto start = std::chrono::system_clock::now();

	emu->Run(halt_exit);

	auto end = std::chrono::system_clock::now();

	emu->Dump();
	if(set.gui) gui->End();

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
