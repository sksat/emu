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
	uint8_t opcode;

	void not_impl_insn();

	void inc_r32(){
		uint8_t r = static_cast<Emulator*>(emu)->GetCode8(0) - 0x40;
		emu->reg[r].reg32++;
		emu->reg[8].reg32++;
	}

	void nop(){ puts("nop"); emu->reg[8].reg32++; }

	void near_jump(){
		int32_t diff = static_cast<Emulator*>(emu)->GetCode32(1);
		emu->reg[8].reg32 += (diff + 5);
	}

	void short_jump(){
		uint8_t diff = (*emu->memory)[emu->reg[8].reg32 + 1];
		emu->reg[8].reg32 += (static_cast<int8_t>(diff) + 2);
	}
};

};

#endif
