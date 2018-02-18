#include "instruction16.h"

#define SETINSN(op,func,insn_flg) {insn[op] = (insnfunc_t)&x86::Instruction16::func; insn_name[op] = #func; insn_flgs[op] = insn_flg;}

namespace x86 {

void Instruction16::Init(){
	Instruction::Init();

	SETINSN(0x05, add_ax_imm16,	Flag::Imm16);
	SETINSN(0x83, code_83,		Flag::ModRM | Flag::Imm8);
	SETINSN(0x8c, mov_rm16_sreg,	Flag::ModRM);
	for(auto i=0;i<8;i++)
		SETINSN(0xb8+i, mov_r16_imm16, Flag::Imm16);
	SETINSN(0xc7, mov_rm16_imm16,	Flag::ModRM | Flag::Imm16);
	SETINSN(0xe8, call_rel16,	Flag::Imm16);
	SETINSN(0xe9, jmp_rel16,	Flag::Imm16);
}

};
