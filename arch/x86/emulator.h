#ifndef X86_EMULATOR_H_
#define X86_EMULATOR_H_

#include "../../emulator_base.h"

#define EIP reg[8].reg32

namespace x86 {

const int REGISTERS_COUNT = 8 + 1;

class Emulator : public EmulatorBase {
public:
	void InitInstructions();
	void InitRegisters();
	void Dump(){ DumpRegisters(); }
	void DumpRegisters();

	uint8_t GetCode8(int index){ return (*memory)[reg[8].reg32 + index]; }
	uint32_t GetCode32(int index){ return (*memory)[reg[8].reg32 + index]; }
};

};

#endif
