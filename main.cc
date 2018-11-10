#include <iostream>
#include "emulator.h"

int main(int argc, char **argv){
	try{
		if(argc != 2){
			std::cout << "usage> ./emu <bin>" << std::endl;
			return -1;
		}

		Emulator emu;
		emu.load_binary(argv[1]);

		std::cout << "emulation start." << std::endl;

		std::cout << "emulation end." << std::endl;

	}catch(std::exception& e){
		std::cerr
			<< "*** EXCEPTION CATCHED ***" << std::endl
			<< e.what() << std::endl
			<< "*************************" << std::endl;
		std::exit(1);
	}
}
