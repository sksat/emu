#ifndef INSN_BASE_H_
#define INSN_BASE_H_

#include <vector>
//#include "_emulator.h"
#include "common.h"

class EmulatorBase;
class InstructionBase;
using insnfunc_t = void (InstructionBase::*)();

class InstructionBase {
private:
	InstructionBase(){}
public:
	InstructionBase(EmulatorBase *e) : emu(e) {}

	void SetEmu(EmulatorBase *emu){ this->emu=emu; }
	virtual void Init() = 0;
	virtual void Parse() = 0;
	virtual void ExecStep() = 0;
protected:
	EmulatorBase *emu;
//	typedef void (InstructionBase::*insnfunc_t)();
	std::vector<insnfunc_t> insn;
};

#endif
