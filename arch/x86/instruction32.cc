#include "instruction32.h"

#define SETINSN(op,func,flg) {insn[op] = (insnfunc_t)&x86::Instruction32::func; insn_flgs[op] = flg;}

namespace x86 {

void Instruction32::Init(){
	Instruction::Init();

	SETINSN(0x01, add_rm32_r32,			Flag::ModRM);
	SETINSN(0x03, add_r32_rm32,			Flag::ModRM);
	SETINSN(0x05, add_eax_imm32,			Flag::Imm32);
	SETINSN(0x25, and_eax_imm32,			Flag::Imm32);
	SETINSN(0x31, xor_rm32_r32,			Flag::ModRM);
	SETINSN(0x39, cmp_rm32_r32,			Flag::ModRM);
	SETINSN(0x3b, cmp_r32_rm32,			Flag::ModRM);
	for(int i=0;i<8;i++){
		SETINSN(0x40+i, inc_r32,		Flag::None);
		SETINSN(0x48+i, dec_r32,		Flag::None);
		SETINSN(0x50+i, push_r32,		Flag::None);
		SETINSN(0x58+i, pop_r32,		Flag::None);
	}
	SETINSN(0x68, push_imm32,			Flag::Imm32);
	SETINSN(0x69, imul_r32_rm32_imm32,		Flag::ModRM | Flag::Imm32); // = imul_r32_imm32
	SETINSN(0x6a, push_imm8,			Flag::Imm8);
	SETINSN(0x81, code_81,				Flag::ModRM | Flag::Imm32);
	SETINSN(0x83, code_83,				Flag::ModRM | Flag::Imm8);
	SETINSN(0x85, test_rm32_r32,			Flag::ModRM);
	SETINSN(0x89, mov_rm32_r32,			Flag::ModRM);
	SETINSN(0x8B, mov_r32_rm32,			Flag::ModRM);
	SETINSN(0x8d, lea_r32_m,			Flag::ModRM);
	SETINSN(0x9c, pushfd,				Flag::None);
	SETINSN(0x9d, popfd,				Flag::None);
	for(int i=0;i<8;i++)
		SETINSN(0xB8+i, mov_r32_imm32,		Flag::Imm32);
	SETINSN(0xC1, code_c1,				Flag::ModRM | Flag::Imm8);
	SETINSN(0xc3, ret32,				Flag::None);
	SETINSN(0xC7, mov_rm32_imm32,			Flag::ModRM | Flag::Imm32);
	SETINSN(0xc9, leave32,				Flag::None);
	SETINSN(0xE8, call_rel32,			Flag::Imm32);
	SETINSN(0xe9, jmp_rel32,			Flag::Imm32); // near jump
	SETINSN(0xea, jmp_ptr16_32,			Flag::Ptr16 | Flag::Imm32); // far jump
	SETINSN(0xff, code_ff,				Flag::ModRM);
}

};
