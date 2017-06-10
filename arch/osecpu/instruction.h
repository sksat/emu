#ifndef OSECPU_INSTRUCTION_H_
#define OSECPU_INSTRUCTION_H_

#include "../../insn_base.h"
#include "emulator.h"

namespace osecpu {

class Instruction : public InstructionBase {
public:
	Instruction(){}
	void Init();
	void ExecStep();
private:
	osecpu::Emulator *emu;

	void Parse();

	void SkipSignature(){ // 0x05
		// if(memory[pc+1]==0xE2 && memory[pc+2]) pc+=3;
	}
};

};

#endif
