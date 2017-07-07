#include <iostream>
#include <sstream>
#include "emulator.h"
#include "instruction.h"

using namespace std;

namespace x86{

void Emulator::InitInstructions(){
//	throw "x86::Emulator::InitInstructions() is not implemented";
	insn = new Instruction(this);
}

void Emulator::InitRegisters(){
//	throw "x86::Emulator::InitRegisters() is not implemented";
	//regs.push_back((uint32_t)0x00);
	reg = std::vector<Register>(REGISTERS_COUNT, (uint32_t)0x00);
	reg[8].name = "EIP";
}

void Emulator::DumpRegisters(){
	stringstream ss;
	ss	<< "--- dump registers ---"	<< endl
		<< "EIP : " << reg[8].reg32	<< endl
		<< "EAX : " << reg[0].reg32 << endl
		<< "ECX : "	<< reg[1].reg32 << endl
		<< "EDX : "	<< reg[2].reg32 << endl
		<< "EBX : " << reg[3].reg32 << endl
		<< "ESP : "	<< reg[4].reg32	<< endl
		<< "EBP : " << reg[5].reg32 << endl
		<< "ESI : "	<< reg[6].reg32 << endl
		<< "EDI : "	<< reg[7].reg32	<< endl
		<< "----------------------"	<< endl;
	cout<<ss.str();
}

};
