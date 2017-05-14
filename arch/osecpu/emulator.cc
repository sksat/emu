#include "emulator.h"
#include "instruction.h"

namespace osecpu {

void Emulator::Init(){
	InitInstructions();
}

void Emulator::InitInstructions(){
//	throw "osecpu::Emulator::InitInstructions() insn is not implemented";
	insn = new osecpu::Instruction();
}

};
