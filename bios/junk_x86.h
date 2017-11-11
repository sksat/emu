#ifndef BIOS_JUNK_X86_H_
#define BIOS_JUNK_X86_H_

#include "junk_base.h"
#include "../arch/x86/emulator.h"

namespace BIOS {
namespace Junk {

class x86 : public BIOS::Junk::Base {
public:
	x86(EmulatorBase *e) : BIOS::Junk::Base(e) {}

	void Boot() override {
		
	}
};

}
}

#endif
