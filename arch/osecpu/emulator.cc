#include "emulator.h"

namespace osecpu {

void Emulator::Init(){
	InitInstructions();
}

void Emulator::InitInstructions(){
	throw "osecpu::Emulator::InitInstructions() insn is not implemented";
}

};
