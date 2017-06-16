#ifndef X86_INSTRUCTION_H_
#define X86_INSTRUCTION_H_

#include <stdio.h>
#include "../../insn_base.h"
#include "emulator.h"

namespace x86 {

class Instruction : public InstructionBase {
public:
	Instruction(Emulator *e) : InstructionBase(e) {}
	void Init();
	void Parse();
	void ExecStep();
private:
	const void not_impl(){ throw "not implemented insn."; }
	const void nop(){ puts("nop"); emu->regs[0].reg32++; }

	uint8_t opcode;
};

};

#endif
