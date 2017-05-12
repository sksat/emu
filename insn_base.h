#ifndef INSN_BASE_H_
#define INSN_BASE_H_

#include <vector>
//#include "_emulator.h"
class Emulator;

class InsnBase {
public:
	InsnBase(){}

	virtual void Init() = 0;
	virtual void Parse() = 0;
	virtual void Exec() = 0;
protected:
	Emulator *emu;
	typedef void (InsnBase::*insnfunc_t)(void);
	std::vector<insnfunc_t> instructions;
};

#endif
