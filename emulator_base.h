#ifndef EMULATOR_BASE_H_
#define EMULATOR_BASE_H_

#include <iostream>
#include <vector>

#include "insn_base.h"
#include "register_base.h"
#include "memory.h"
#include "device/device.h"
#include "bios/base.h"

class EmulatorBase {
public:
	EmulatorBase();
	virtual ~EmulatorBase(); // 継承前提のクラスのデストラクタはvirtual
	void Init();
	void SetBios(BIOS::Base *bios);
	virtual void InitInstructions() = 0;
	virtual void InitRegisters() = 0;
	virtual void InitMemory() = 0;

	virtual void ConnectDevice(Device::Base &dev);

	virtual void Run(){
		while(!finish_flg){
			if(!halt_flg) insn->ExecStep();
		}
	}

	virtual void Dump();
	virtual void DumpRegisters();
	virtual void DumpMemory();

	bool halt_flg	= false;
	bool finish_flg = false;

	InstructionBase *insn;
	std::vector<RegisterBase*> all_reg;
	std::vector<Device::Base*> dev;
	Memory *memory;
	BIOS::Base *bios;
};

#endif
