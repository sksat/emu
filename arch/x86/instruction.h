#ifndef X86_INSTRUCTION_H_
#define X86_INSTRUCTION_H_

#include <stdio.h>
#include "../../insn_base.h"
#include "emulator.h"
#include "insndata.h"

namespace x86 {

class Instruction : public InstructionBase {
public:
	Instruction(x86::Emulator *e);
	~Instruction(){ delete idata; }
	virtual void Init();
	virtual void Parse() = 0;
	void ExecStep();
protected:
	x86::Emulator *emu;
	x86::InsnData *idata;
	struct Flag {
		static const uint8_t modrm = 0b0001;
	};
	uint8_t insn_flgs[256];

	void not_impl_insn();

#define DEFINE_JX(flag, is_flag) \
void j ## flag(){ \
	emu->EIP += (emu->eflags.is_flag() ? emu->GetSignCode8(1) : 0) + 2; \
} \
void jn ## flag(){ \
	emu->EIP += (emu->eflags.is_flag() ? 0 : emu->GetSignCode8(1)) + 2; \
}

	DEFINE_JX(o, IsOverflow);
	DEFINE_JX(c, IsCarry);
	DEFINE_JX(z, IsZero);
	DEFINE_JX(s, IsSign);

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
