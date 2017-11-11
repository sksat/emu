#ifndef EMULATOR_H_
#define EMULATOR_H_

#include "common.h"
#include "arch/arch.h"
#include "insn_base.h"
#include "emulator_base.h"
#include "memory.h"
#include "device/floppy.h"
#include "bios/base.h"
#include "bios/junk_x86.h"

#define DEFAULT_MEMORY_SIZE	1 // MB

class EmulatorCtrl {
public:
	struct Setting {
		ARCH arch;
		size_t memsize; //MB
		bool junk_bios;
		bool gui;
		bool fullscreen;
	};

	EmulatorCtrl(){
		set = {
			.arch = ARCH::x86,
			.memsize = DEFAULT_MEMORY_SIZE,
			.junk_bios = false,
			.gui = true,
			.fullscreen = false
		};
	}
//	Emulator(ARCH arch){ SetArch(static_cast<int>(arch)); }
//	Emulator(int arch){  SetArch(arch); }

	EmulatorBase* operator->(){
		if(emu == nullptr) exit(-1);
		return emu;
	}

//	EmulatorBase* operator->*(EmulatorBase* U::*) const noexcept ;

	void Init(){
		Init(this->set);
	}

	void Init(EmulatorCtrl::Setting set){
		SetArch(set.arch);
		this->set = set;
	}

	void SetArch(ARCH arch){
		this->set.arch = arch;
		switch(arch){
		case ARCH::x86:
			emu = new x86::Emulator();
			break;
		case ARCH::osecpu:
			emu = new osecpu::Emulator();
			break;
		default:
			// unkown arch
			throw "error: Emulator: unkown arch";
		}
		emu->Init();
	}

	EmulatorBase* GetRaw(){ return emu; }

private:
	EmulatorCtrl::Setting set;
	EmulatorBase *emu;
};

#endif
