#ifndef BIOS_JUNK_BASE_H_
#define BIOS_JUNK_BASE_H_

#include "../emulator_base.h"

namespace BIOS {

namespace Junk {

class Base : public BIOS::Base {
public:
	Base(EmulatorBase *e) : BIOS::Base(e) {}
	virtual void Boot() = 0;
protected:
	EmulatorBase *emu;
};

}
}

#endif
