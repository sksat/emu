#include <iostream>

int main(int argc, char **argv){
	if(argc != 2){
		std::cout << "usage> ./emu <bin>" << std::endl;
		return -1;
	}

	std::cout << "emulation start." << std::endl;

	std::cout << "emulation end." << std::endl;
}
