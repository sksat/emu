#ifndef X86_INSTRUCTION_H_
#define X86_INSTRUCTION_H_

#include <stdio.h>
#include "../../insn_base.h"
#include "emulator.h"
#include "modrm.h"

namespace x86 {

class Instruction : public InstructionBase {
public:
	Instruction(x86::Emulator *e);
	~Instruction(){ delete modrm; }
	virtual void Init();
	virtual void Parse() = 0;
	void ExecStep();
protected:
	x86::Emulator *emu;
	x86::ModRM *modrm;
	friend x86::ModRM;
	bool insn_flgs[0xff];
	uint8_t prefix, opcode, sib;

	void not_impl_insn();
	void nop(){ puts("nop"); emu->EIP++; }

	void near_jump(){
		int32_t diff = emu->GetCode32(1);
		emu->EIP += (diff + 5);
	}

	void short_jump(){
		uint8_t diff = (*emu->memory)[emu->EIP + 1];
		emu->EIP += (static_cast<int8_t>(diff) + 2);
	}
};

};

#endif
