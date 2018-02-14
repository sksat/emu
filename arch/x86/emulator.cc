#include <iostream>
#include <sstream>
#include "emulator.h"
#include "register.h"
#include "insndata.h"
#include "instruction.h"
#include "instruction16.h"
#include "instruction32.h"

using namespace std;

namespace x86{

void Emulator::InitInstructions(){
	// 起動時は16bitリアルモード
	idata = new InsnData(this);
	insn16 = new Instruction16(this, idata);
	insn32 = new Instruction32(this, idata);
	insn16->Init();
	insn32->Init();
	insn = insn16;
	mode = 16;
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

	sreg = std::vector<x86::SRegister>(SREG_COUNT);
	const char* sreg_name[SREG_COUNT] = {
		"ES", "CS", "SS", "DS", "FS", "GS",
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

void Emulator::RunStep(){
	bool is_16 = IsMode16();

	insn->Fetch();
	idata->chsiz_addr = !(is_16 ^ idata->chsiz_addr);
	if(is_16 ^  idata->chsiz_op){
		insn16->Decode();
		insn16->Exec();
	}else{
		insn32->Decode();
		insn32->Exec();
	}

	DOUT(std::endl);
}

}
