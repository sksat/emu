#ifndef INSN_BASE_H_
#define INSN_BASE_H_

#include <string>
#include <vector>
#include "common.h"

class EmulatorBase;
class InstructionBase;
using insnfunc_t = void (InstructionBase::*)();

class InstructionBase {
public:
	InstructionBase(){
/* ✟私はコンストラクタで純粋仮想関数を使おうとしました✟ */
		//insn = std::vector<insnfunc_t>(0xff, (insnfunc_t)&InstructionBase::not_impl_insn);
	}
	//InstructionBase(EmulatorBase *e) : emu(e) {}
	virtual ~InstructionBase(){}

	//void SetEmu(EmulatorBase *emu){ this->emu=emu; }
	void ClearInsn(size_t num){ insn.resize(num, (insnfunc_t)&InstructionBase::not_impl_insn); }
	virtual void Init() = 0;
	virtual void Parse() = 0;
	virtual void ExecStep() = 0;
protected:
	//EmulatorBase *emu;
//	typedef void (InstructionBase::*insnfunc_t)();
	std::vector<insnfunc_t> insn;
	std::vector<std::string> insn_name;
	virtual void not_impl_insn() = 0;
};

#endif
