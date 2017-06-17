#ifndef INSN_BASE_H_
#define INSN_BASE_H_

#include <vector>
//#include "_emulator.h"
#include "common.h"

#define SETINSN(o,f) (insn[o] = (insnfunc_t)&Instruction::f)

class EmulatorBase;
class InstructionBase;
using insnfunc_t = void (InstructionBase::*)();

class InstructionBase {
private:
	InstructionBase(){
/* ✟私はコンストラクタで純粋仮想関数を使おうとしました✟ */
		//insn = std::vector<insnfunc_t>(0xff, (insnfunc_t)&InstructionBase::not_impl_insn);
	}
public:
	InstructionBase(EmulatorBase *e) : emu(e) {}

	void SetEmu(EmulatorBase *emu){ this->emu=emu; }
	void ClearInsn(size_t num){ insn = std::vector<insnfunc_t>(num, (insnfunc_t)&InstructionBase::not_impl_insn); }
	virtual void Init() = 0;
	virtual void Parse() = 0;
	virtual void ExecStep() = 0;
protected:
	EmulatorBase *emu;
//	typedef void (InstructionBase::*insnfunc_t)();
	std::vector<insnfunc_t> insn;
	virtual void not_impl_insn() = 0;
};

#endif
