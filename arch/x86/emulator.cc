#include <iostream>
#include <sstream>
#include "emulator.h"
#include "register.h"
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
	pc.SetName("EIP");

	reg = std::vector<x86::Register>(REGISTERS_COUNT);
/*
	reg[8].name = "EIP";
	reg[0].name = "EAX";
	reg[1].name = "ECX";
	reg[2].name = "EDX";
	reg[3].name = "EBX";
	reg[4].name = "ESP";
	reg[5].name = "EBP";
	reg[6].name = "ESI";
	reg[7].name = "EDI";
*/
	const char* reg_name[REGISTERS_COUNT] = {
		"EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI",
	};
	for(int i=0;i<REGISTERS_COUNT;i++){
		reg[i].SetName(reg_name[i]);
	}

// all_regへの登録
	all_reg = std::vector<::RegisterBase*>(REGISTERS_COUNT + 1);
	for(int i=0;i<REGISTERS_COUNT;i++)
		all_reg[i] = &reg[i];
	all_reg[REGISTERS_COUNT] = &pc;

	// EIPとESPの初期設定(ここでやるべきではない)
	EIP = ESP = 0x7c00;
}

void Emulator::InitMemory(){
	memory->endian = ENDIAN::LITTLE;
}

}
