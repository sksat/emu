#include <iostream>
#include "emulator.h"
#include "util.h"

void Emulator::dump_registers(){
	using std::endl;
	std::cout
		<< "----- dump registers-----" << endl
		<< "EIP: " << hex2str(EIP) << endl
		<< "EAX: " << hex2str(EAX) << endl
		<< "ECX: " << hex2str(ECX) << endl
		<< "EDX: " << hex2str(EDX) << endl
		<< "EBX: " << hex2str(EBX) << endl
		<< "ESP: " << hex2str(ESP) << endl
		<< "EBP: " << hex2str(EBP) << endl
		<< "ESI: " << hex2str(ESI) << endl
		<< "EDI: " << hex2str(EDI) << endl
		<< "-------------------------" << std::endl;
}
