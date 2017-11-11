#ifndef BIOS_JUNK_X86_H_
#define BIOS_JUNK_X86_H_

#include <iostream>
#include <typeinfo>
#include "junk_base.h"
#include "../arch/x86/emulator.h"

namespace BIOS {
namespace Junk {

class x86 : public BIOS::Junk::Base {
public:
	x86(EmulatorBase *e) : BIOS::Junk::Base(e) {
		if(typeid(*e) != typeid(::x86::Emulator))
			throw "emulator is not for x86";
		emu = dynamic_cast<::x86::Emulator*>(e);
	}

	void Boot() {
		using namespace std;
		cout<<"booting x86 BIOS..."<<endl;
		// TODO: load IPL
		emu->EIP = 0x7c00;
		emu->ESP = 0x7c00;
	}
private:
	::x86::Emulator *emu;
};

}
}

#endif
