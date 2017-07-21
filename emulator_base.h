#ifndef EMULATOR_BASE_H_
#define EMULATOR_BASE_H_

#include <iostream>
#include <vector>

#include "insn_base.h"
#include "register.h"
#include "memory.h"

class EmulatorBase {
public:
	EmulatorBase();
	~EmulatorBase();
	virtual void Init();
	virtual void InitInstructions() = 0;
	virtual void InitRegisters() = 0;
	virtual void InitMemory() = 0;
	virtual void Dump() = 0;
	virtual void DumpRegisters();

	bool finish_flg = false;

	InstructionBase *insn;
	std::vector<Register> reg;
	Memory *memory;
};

#endif
