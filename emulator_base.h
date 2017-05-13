#ifndef EMULATOR_BASE_H_
#define EMULATOR_BASE_H_

#include "insn_base.h"
#include "memory.h"

class EmulatorBase {
public:
	virtual void Init() = 0;

	virtual void InitInstructions() = 0;

	InstructionBase *insn;
	Memory memory;
};

#endif
