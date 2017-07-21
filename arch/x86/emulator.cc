#include <iostream>
#include <sstream>
#include "emulator.h"
#include "instruction.h"
#include "instruction16.h"
#include "instruction32.h"

using namespace std;

namespace x86{

void Emulator::InitInstructions(){
//	throw "x86::Emulator::InitInstructions() is not implemented";
	insn = new Instruction32(this);
}

void Emulator::InitRegisters(){
//	throw "x86::Emulator::InitRegisters() is not implemented";
	//regs.push_back((uint32_t)0x00);
	reg = std::vector<Register>(REGISTERS_COUNT, (uint32_t)0x00);
	reg[8].name = "EIP";
	reg[0].name = "EAX";
	reg[1].name = "ECX";
	reg[2].name = "EDX";
	reg[3].name = "EBX";
	reg[4].name = "ESP";
	reg[5].name = "EBP";
	reg[6].name = "ESI";
	reg[7].name = "EDI";
}

void Emulator::InitMemory(){
	memory->endian = ENDIAN::LITTLE;
}

}
