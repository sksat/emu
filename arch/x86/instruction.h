#ifndef X86_INSTRUCTION_H_
#define X86_INSTRUCTION_H_

#include <vector>
#include "../../insn_base.h"

namespace x86 {

class Instruction : public InstructionBase {
public:
	Instruction(){}
	void Init();
	void Parse();
	void StepExec();
};

};

#endif
