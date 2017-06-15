#ifndef X86_INSTRUCTION_H_
#define X86_INSTRUCTION_H_

#include <stdio.h>
#include <vector>
#include "../../insn_base.h"

namespace x86 {

class Instruction : public InstructionBase {
public:
	void Init();
	void Parse();
	void ExecStep();
private:
	void not_impl(){ throw "not implemented insn."; }
	const void nop(){ puts("nop"); }

	uint8_t opcode;
};

};

#endif
