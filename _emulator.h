#ifndef EMULATOR_H_
#define EMULATOR_H_

#include "emulator_base.h"
#include "arch/arch.h"

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
		case ARCH::x86:
			this->arch = ARCH::x86;
			emu = new x86::Emulator();
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
