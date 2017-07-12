#include <sstream>
#include "instruction.h"
#include "emulator.h"

namespace x86 {

void Instruction::Init(){
	// default insn
//	insn = std::vector<insnfunc_t>(0xff, (insnfunc_t)&Instruction::not_impl_insn);
	ClearInsn(0xff);
	opcode = 0x90;

//	for(i=0;i<8;i++)
//		SETINSN(0x40 + i, inc_r32);
	SETINSN(0x90, nop);
	SETINSN(0xe9, near_jump);
	SETINSN(0xeb, short_jump);
}

void Instruction::Parse(){
	static int a=0;
	if(a!=0){
		if(emu->reg[8].reg32 == 0) emu->finish_flg=true;
	}
	a++;
	opcode = (*emu->memory)[(uint32_t)emu->reg[8]];
}

void Instruction::ExecStep(){
	Parse();
	insnfunc_t func = insn[opcode];
	(this->*func)();
}

void Instruction::not_impl_insn(){
	std::stringstream ss;
	ss<<"x86: not implemented insn : "<<std::hex<<std::showbase<<static_cast<uint32_t>(opcode);
	throw ss.str();
}

};
