#ifndef INSN_BASE_H_
#define INSN_BASE_H_

#include <vector>
//#include "_emulator.h"
#include "common.h"

class Emulator;
class InstructionBase;
using insnfunc_t = void (InstructionBase::*)();

class InstructionBase {
public:
	InstructionBase(){}

	virtual void Init() = 0;
	virtual void Parse() = 0;
	virtual void ExecStep() = 0;
protected:
	Emulator *emu;
//	typedef void (InstructionBase::*insnfunc_t)();
	std::vector<insnfunc_t> insn;
};

#endif
