#include "instruction.h"

namespace x86 {

void Instruction::Init(){
	// default insn
	insn = std::vector<insnfunc_t>(0xff, (insnfunc_t)&Instruction::not_impl);
	opcode = 0x90;

	insn[0x90] = (insnfunc_t)&Instruction::nop;
}

void Instruction::Parse(){
	
}

void Instruction::ExecStep(){
	insnfunc_t func = insn[opcode];
	(this->*func)();
}

};
