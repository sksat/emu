#include "instruction32.h"

namespace x86 {

void Instruction32::Init(){
	Instruction::Init();
	for(int i=0;i<8;i++)
		SETINSN(0x40+i, inc_r32);
	SETINSN(0xC7, mov_rm32_imm32);
}

void Instruction32::Parse(){
	Instruction::Parse();
}

};
