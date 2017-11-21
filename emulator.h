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

	void Init(){
		Init(this->set);
	}

	void Init(const EmulatorCtrl::Setting set){
		SetArch(set.arch);
		this->set = set;
	}

	void SetArch(const ARCH arch){
		this->set.arch = arch;
		switch(arch){
		case ARCH::x86:
			emu = std::make_unique<x86::Emulator>();
			break;
		case ARCH::osecpu:
			emu = std::make_unique<osecpu::Emulator>();
			break;
		default:
			// unkown arch
			throw "error: Emulator: unkown arch";
		}
		emu->Init();
	}

	EmulatorBase* GetRaw(){ return emu.get(); }

private:
	EmulatorCtrl::Setting set;
	std::unique_ptr<EmulatorBase> emu;
};

#endif
