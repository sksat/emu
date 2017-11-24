#ifndef X86_INSTRUCTION16_H_
#define X86_INSTRUCTION16_H_

#include "instruction.h"

namespace x86 {

class Instruction16 : public x86::Instruction {
public:
	Instruction16(x86::Emulator *e) : x86::Instruction(e) {}
	void Init();
	void Parse();
};

};

#endif
