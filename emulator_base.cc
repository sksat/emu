#include <iomanip>
#include "emulator_base.h"

void EmulatorBase::Init(){
	finish_flg = false;
	InitInstructions();
	InitRegisters();
}

void EmulatorBase::DumpRegisters(){
	std::cout
		<< "---- dump registers ----"
		<< std::endl;
	for(int i=0;i<reg.size();i++){
		std::cout
			<< reg[i].name << " : "
			<< "0x"
		//	<< std::showbase	// <- if 0, dosen't work
			<< std::hex
			<< std::setw(4)
			<< std::setfill('0')
			<< reg[i].reg32
			<< std::endl;
	}
	std::cout
		<< "------------------------"
		<< std::endl;
}
