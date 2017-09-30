#ifndef ARCH_H_
#define ARCH_H_

enum class ARCH{
	x86,
	osecpu,
};

#include "x86/emulator.h"
#include "x86/instruction.h"
#include "osecpu/emulator.h"
#include "osecpu/instruction.h"

#endif // ARCH_H_
