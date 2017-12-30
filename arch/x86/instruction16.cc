#include "instruction16.h"

#define SETINSN(op,func,insn_flg) {insn[op] = (insnfunc_t)&x86::Instruction16::func; insn_name[op] = #func; insn_flgs[op] = insn_flg;}

namespace x86 {

void Instruction16::Init(){
	Instruction::Init();
	
	for(auto i=0;i<8;i++)
		SETINSN(0xb8+i, mov_r16_imm16, 0);
}

void Instruction16::Parse(){
	Instruction::Parse();
}

};
