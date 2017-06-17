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
	const void not_impl();
	void nop(){ puts("nop"); emu->regs[0].reg32++; }
	void short_jump(){
		uint8_t diff = (*emu->memory)[emu->regs[0].reg32 + 1];
		emu->regs[0].reg32 += (static_cast<int8_t>(diff) + 2);
	}

	uint8_t opcode;
};

};

#endif
