#ifndef EMULATOR_H_
#define EMULATOR_H_

#include "common.h"
#include "arch/arch.h"
#include "insn_base.h"
#include "emulator_base.h"
#include "memory.h"

#define DEFAULT_MEMORY_SIZE	1 * MB

#define CASE(a) case (static_cast<int>(a))

class Emulator {
public:
	Emulator(ARCH arch){ SetArch(static_cast<int>(arch)); }
	Emulator(int arch){  SetArch(arch); }

	EmulatorBase* operator->(){
		return emu;
	}

//	EmulatorBase* operator->*(EmulatorBase* U::*) const noexcept ;

	void Init(){
		emu->Init();
	}

	void SetArch(int arch){
		if(emu != nullptr) delete emu;
		switch(arch){
		CASE(ARCH::x86):
			this->arch = ARCH::x86;
			emu = new x86::Emulator();
			insn = new x86::Instruction();
			insn->Init();
//			throw "x86";
			break;
		CASE(ARCH::osecpu):
			this->arch = ARCH::osecpu;
			emu = new osecpu::Emulator();
			insn = new osecpu::Instruction();
			insn->Init();
			break;
		default:
			// unkown arch
			throw "error: Emulator: unkown arch";
		}
		emu->Init();
	}

private:
	ARCH arch;
	InsnBase *insn;
	EmulatorBase *emu;
};

#endif
