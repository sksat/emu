#include "instruction32.h"

namespace x86 {

void Instruction32::Init(){
	Instruction::Init();

//	SETINSN(0x01, add_rm32_r32,		1);
	for(int i=0;i<8;i++)
		SETINSN(0x40+i, inc_r32,	0);
	SETINSN(0x89, mov_rm32_r32,		1);
	SETINSN(0xC7, mov_rm32_imm32,	1);
}

void Instruction32::Parse(){
	Instruction::Parse();
}

};
