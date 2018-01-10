#ifndef X86_INSTRUCTION16_H_
#define X86_INSTRUCTION16_H_

#include "instruction.h"

namespace x86 {

class Instruction16 : public x86::Instruction {
public:
	Instruction16(x86::Emulator *e) : x86::Instruction(e) {}
	void Init();

	void mov_r16_imm16(){
		uint8_t num = idata->opcode & ((1<<3)-1);
		auto& reg = emu->reg[num];
		reg = idata->imm16;
		DOUT(std::endl<<"\t"<<reg.GetName()<<"=0x"<<idata->imm16);
	}
};

};

#endif
