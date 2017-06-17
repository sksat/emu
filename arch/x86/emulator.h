#ifndef X86_EMULATOR_H_
#define X86_EMULATOR_H_

#include "../../emulator_base.h"

namespace x86 {

const int REGISTERS_COUNT = 8;

class Emulator : public EmulatorBase {
public:
	void InitInstructions();
	void InitRegisters();
	void Dump(){ std::cout<<"dump"<<std::endl; }
};

};

#endif
