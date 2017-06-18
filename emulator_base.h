#ifndef EMULATOR_BASE_H_
#define EMULATOR_BASE_H_

#include <iostream>
#include <vector>

#include "insn_base.h"
#include "register.h"
#include "memory.h"

class EmulatorBase {
public:
	void Init();
	virtual void InitInstructions() = 0;
	virtual void InitRegisters() = 0;
	virtual void Dump() = 0;

	bool finish_flg = false;

	InstructionBase *insn;
	std::vector<Register> reg;
	Memory *memory;
};

#endif
