#ifndef X86_EMULATOR_H_
#define X86_EMULATOR_H_

#include "../../emulator_base.h"
#include "instruction.h"

namespace x86 {

class Emulator : public EmulatorBase {
public:
	void InitInstructions();
	void InitRegisters();
};

};

#endif
