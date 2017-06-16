#include "emulator.h"
#include "instruction.h"

namespace x86{

void Emulator::InitInstructions(){
//	throw "x86::Emulator::InitInstructions() is not implemented";
	insn = new Instruction(this);
}

void Emulator::InitRegisters(){
//	throw "x86::Emulator::InitRegisters() is not implemented";
	regs.push_back((uint32_t)0x00);
}

};
