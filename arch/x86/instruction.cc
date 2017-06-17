#include <sstream>
#include "instruction.h"
#include "emulator.h"

namespace x86 {

void Instruction::Init(){
	// default insn
//	insn = std::vector<insnfunc_t>(0xff, (insnfunc_t)&Instruction::not_impl_insn);
	opcode = 0x90;

	SETINSN(0x90, nop);
	SETINSN(0xeb, short_jump);
}

void Instruction::Parse(){
	opcode = (*emu->memory)[(uint32_t)emu->regs[0]];
}

void Instruction::ExecStep(){
	Parse();
	insnfunc_t func = insn[opcode];
	(this->*func)();
}

void Instruction::not_impl_insn(){
	std::stringstream ss;
	ss<<"not implemented insn : "<<std::hex<<std::showbase<<static_cast<uint32_t>(opcode);
	throw ss.str();
}

};
