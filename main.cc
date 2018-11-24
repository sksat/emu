#include <iostream>
#include "emulator.h"
#include "insn.h"

int main(int argc, char **argv){
	Emulator emu;
	try{
		if(argc != 2){
			std::cout << "usage> ./emu <bin>" << std::endl;
			return -1;
		}

		auto& cpu = emu.cpu;
		auto& memory = emu.memory;
		cpu.reg_pc.r32 = 0x00;
		memory.load_binary(0x00, argv[1]);
		EIP = 0x00;
		insn::init();

		std::cout << "emulation start." << std::endl;

		while(!cpu.halt_flag){
			cpu.fetch_decode(memory);
			cpu.exec(memory);
		}

		std::cout << "emulation end." << std::endl;
		emu.dump_registers();

	}catch(std::exception& e){
		std::cerr
			<< "*** EXCEPTION CATCHED ***" << std::endl
			<< e.what() << std::endl
			<< "*************************" << std::endl;
		emu.dump_registers();
		std::exit(1);
	}
}
