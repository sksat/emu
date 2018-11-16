#include <iostream>
#include "emulator.h"

int main(int argc, char **argv){
	Emulator emu;
	try{
		if(argc != 2){
			std::cout << "usage> ./emu <bin>" << std::endl;
			return -1;
		}

		emu.load_binary(argv[1]);
		auto& cpu = emu.cpu;
		auto& memory = emu.memory;
		cpu.reg_pc.r32 = 0x00;
		EIP = 0x00;

		std::cout << "emulation start." << std::endl;

		while(true){
			cpu.fetch(memory);
			EIP++;
		}

		std::cout << "emulation end." << std::endl;

	}catch(std::exception& e){
		std::cerr
			<< "*** EXCEPTION CATCHED ***" << std::endl
			<< e.what() << std::endl
			<< "*************************" << std::endl;
		emu.dump_registers();
		std::exit(1);
	}
}
