#include "emulator.h"

namespace x86{

void Emulator::Init(){
	InitInstructions();
}

void Emulator::InitInstructions(){
	throw "x86::Emulator::InitInstructions() insn is not implemented";
}

};
