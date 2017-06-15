#include "emulator.h"

namespace x86{

void Emulator::InitInstructions(){
//	throw "x86::Emulator::InitInstructions() is not implemented";
	insn = new Instruction();
	insn->SetEmu(this);
}

void Emulator::InitRegisters(){
//	throw "x86::Emulator::InitRegisters() is not implemented";
}

};
