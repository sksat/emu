#ifndef OSECPU_INSTRUCTION_H_
#define OSECPU_INSTRUCTION_H_

#include "../../insn_base.h"
#include "emulator.h"

namespace osecpu {

class Instruction : public InstructionBase {
public:
	Instruction(Emulator *e) {}
	void Init();

	void Fetch();
	void Decode();
	void Exec();

	void SkipSignature(){ // 0x05
		// if(memory[pc+1]==0xE2 && memory[pc+2]) pc+=3;
	}

	uint8_t opcode;
	uint8_t insn_flgs[0xff];

	void not_impl_insn(){ throw "osecpu: not implemented insn."; }

	void nop(){ puts("osecpu: nop"); }
};

};

#endif
