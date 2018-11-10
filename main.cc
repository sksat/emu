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
		EIP = 0x00;

		std::cout << "emulation start." << std::endl;

		while(true){
			auto op = memory[EIP++];
			std::cout
				<< "EIP="<< std::hex << EIP
				<< " opcode: " << static_cast<uint32_t>(op) << std::endl;
			switch(op){
				case 0xeb:
					std::cout << "\tshort jmp!" << std::endl;
					{
						int8_t rel8 = memory[EIP++];
						std::cout<<"rel8="<<std::dec<<(int)rel8<<std::endl;
						EIP = EIP + static_cast<int32_t>(rel8);
					}
					break;
				default:
					throw std::runtime_error("unknwon opcode");
			}
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
