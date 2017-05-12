#ifndef EMULATOR_BASE_H_
#define EMULATOR_BASE_H_

#include "memory.h"

class EmulatorBase {
public:
	virtual void Init() = 0;

	Memory memory;
};

#endif
