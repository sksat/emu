#include "instruction.h"

#define SETINSN(op,func,insn_flg) {insn[op] = (insnfunc_t)&osecpu::Instruction::func; insn_flgs[op] = insn_flg;}

namespace osecpu {

void Instruction::Init(){
	throw "osecpu::Instruction::Init() is not implemented.";
	ClearInsn(0xff);
	opcode = 0x00;

	SETINSN(0x00, nop, 0);
}

void Instruction::Fetch(){
	throw "osecpu::Instruction::Fetch() is not implemented.";
}

void Instruction::Decode(){
	throw "osecpu::Instruction::Decode() is not implemented.";
}

void Instruction::Exec(){
	throw "osecpu::Instruction::Exec() is not implemented.";
}

};
