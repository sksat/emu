#ifndef EMULATOR_BASE_H_
#define EMULATOR_BASE_H_

#include <iostream>
#include <vector>

#include "insn_base.h"
#include "register_base.h"
#include "memory.h"
#include "device/device.h"

class EmulatorBase {
public:
	EmulatorBase();
	~EmulatorBase();
	void Init();
	virtual void InitInstructions() = 0;
	virtual void InitRegisters() = 0;
	virtual void InitMemory() = 0;

	virtual void ConnectDevice(Device::Base* dev);
	virtual void Dump();
	virtual void DumpRegisters();
	virtual void DumpMemory();

	bool finish_flg = false;

	InstructionBase *insn;
	std::vector<RegisterBase*> all_reg;
	std::vector<Device::Base*> dev;
	Memory *memory;
};

#endif
