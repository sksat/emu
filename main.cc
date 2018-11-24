#include <iostream>
#include "cpu.h"
#include "memory.h"
#include "insn.h"

int main(int argc, char **argv){
	CPU cpu;
	auto memory = std::make_shared<Memory>();
	try{
		if(argc != 2){
			std::cout << "usage> ./emu <bin>" << std::endl;
			return -1;
		}

		cpu.reg_pc.r32 = 0x00;
		cpu.memory = memory;
		memory->load_binary(0x00, argv[1]);
		EIP = 0x00;
		insn::init();

		std::cout << "emulation start." << std::endl;

		while(!cpu.halt_flag){
			cpu.fetch_decode();
			cpu.exec();
		}

		std::cout << "emulation end." << std::endl;
		cpu.dump_registers();

	}catch(std::exception& e){
		std::cerr
			<< "*** EXCEPTION CATCHED ***" << std::endl
			<< e.what() << std::endl
			<< "*************************" << std::endl;
		cpu.dump_registers();
		std::exit(1);
	}
}
