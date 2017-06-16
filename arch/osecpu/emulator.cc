#include "emulator.h"
#include "instruction.h"

namespace osecpu {

void Emulator::InitInstructions(){
//	throw "osecpu::Emulator::InitInstructions() is not implemented";
	insn = new osecpu::Instruction(this);
}

void Emulator::InitRegisters(){
	throw "osecpu::Emulator::InitRegisters() is not implemented";
}

};
