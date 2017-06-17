#ifndef OSECPU_EMULATOR_H_
#define OSECPU_EMULATOR_H_

#include "../../emulator_base.h"

namespace osecpu {

class Emulator : public EmulatorBase {
public:
	void InitInstructions();
	void InitRegisters();
	void Dump();
};

};

#endif
