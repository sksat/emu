#include "instruction.h"
#include "emulator.h"

namespace x86 {

void Instruction::Init(){
	// default insn
	insn = std::vector<insnfunc_t>(0xff, (insnfunc_t)&Instruction::not_impl);
	opcode = 0x90;

	insn[0x90] = (insnfunc_t)&Instruction::nop;
}

void Instruction::Parse(){
	opcode = (*emu->memory)[emu->regs[0].reg32];
}

void Instruction::ExecStep(){
	Parse();
	insnfunc_t func = insn[opcode];
	(this->*func)();
}

};
