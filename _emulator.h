#ifndef EMULATOR_H_
#define EMULATOR_H_

#include "emulator_base.h"
#include "arch/arch.h"

#define CASE(a) case (static_cast<int>(a))

class Emulator {
public:
	Emulator(ARCH arch){ SetArch(static_cast<int>(arch)); }
	Emulator(int arch){  SetArch(arch); }

	EmulatorBase* operator->(){
		return emu;
	}

	void SetArch(int arch){
		if(emu != nullptr) delete emu;
		switch(arch){
		CASE(ARCH::x86):
			this->arch = ARCH::x86;
			emu = new x86::Emulator();
//			throw "x86";
			break;
		default:
			// unkown arch
			throw "error: Emulator: unkown arch";
		}
		emu->Init();
	}
private:
	ARCH arch;
	EmulatorBase *emu;
};

#endif
