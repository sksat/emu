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
	// 起動時は16bitリアルモード
	insn = new Instruction16(this);
	insn->Init();
}

void Emulator::InitRegisters(){
	pc.SetName("EIP");
	eflags.SetName("EFLAGS");

	reg = std::vector<x86::Register32>(REG_COUNT);
	const char* reg_name[REG_COUNT] = {
		"EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI",
	};
	for(size_t i=0;i<REG_COUNT;i++){
		reg[i].SetName(reg_name[i]);
	}

	sreg = std::vector<x86::Register16>(SREG_COUNT);
	const char* sreg_name[SREG_COUNT] = {
		"CS", "DS", "SS", "ES", "FS", "GS",
	};
	for(size_t i=0;i<SREG_COUNT;i++)
		sreg[i].SetName(sreg_name[i]);

// all_regへの登録
	all_reg.push_back(&pc);
	all_reg.push_back(&eflags);
	for(size_t i=0;i<REG_COUNT;i++)
		all_reg.push_back(&reg[i]);
	for(size_t i=0;i<SREG_COUNT;i++)
		all_reg.push_back(&sreg[i]);
}

void Emulator::InitMemory(){
	memory->endian = ENDIAN::LITTLE;
}

}
