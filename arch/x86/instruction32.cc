#include "instruction32.h"

#define SETINSN(op,func,flg) {insn[op] = (insnfunc_t)&x86::Instruction32::func; insn_flgs[op] = flg;}

namespace x86 {

void Instruction32::Init(){
	Instruction::Init();

//	SETINSN(0x01, add_rm32_r32,			1);
	SETINSN(0x25, and_eax_imm32,			Flag::Imm32);
//	SETINSN(0x3B, cmp_r32_rm32,			1);
//	for(int i=0;i<8;i++)
//		SETINSN(0x40+i, inc_r32,		0);
//	for(int i=0;i<8;i++){
//		SETINSN(0x50+i, push_r32,		0);
//		SETINSN(0x58+i, pop_r32,		0);
//	}
	SETINSN(0x69, imul_r32_rm32_imm32,		Flag::ModRM | Flag::Imm32); // = imul_r32_imm32
	SETINSN(0x83, code_83,				Flag::ModRM | Flag::Imm8);
	SETINSN(0x89, mov_rm32_r32,			Flag::ModRM);
	SETINSN(0x8B, mov_r32_rm32,			Flag::ModRM);
	for(int i=0;i<8;i++)
		SETINSN(0xB8+i, mov_r32_imm32,		Flag::Imm32);
//	SETINSN(0xC3, ret32,				0);
	SETINSN(0xC7, mov_rm32_imm32,			Flag::ModRM | Flag::Imm32);
//	SETINSN(0xC9, leave32,				0);
//	SETINSN(0xE8, call_rel32,			0);
//	SETINSN(0xFF, code_ff,				1);
}

};
