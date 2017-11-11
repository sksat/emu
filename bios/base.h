#ifndef BIOS_H_
#define BIOS_H_

#include "../emulator_base.h"

namespace BIOS {

class Base {
public:
	Base(EmulatorBase *e) : emu_base(e) { if(e==nullptr) throw "ぬるぽ"; }
	virtual void Boot() = 0;
protected:
	EmulatorBase *emu_base;
};

}

#endif
