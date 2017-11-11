#ifndef BIOS_JUNK_X86_H_
#define BIOS_JUNK_X86_H_

#include <iostream>
#include <typeinfo>
#include "junk_base.h"
#include "../arch/x86/emulator.h"
#include "../device/floppy.h"

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

		LoadIPL();

		emu->EIP = 0x7c00;
		emu->ESP = 0x7c00;
	}

	Device::Base* GetBootDevice(){
		auto& dev = emu->dev;
		for(size_t i=0; i<dev.size(); i++){
			if(typeid(*dev[i]) == typeid(Device::Floppy))
				return dev[i];
		}
		throw "No bootable device.";
	}

	void LoadIPL(){
		auto d = GetBootDevice();
		if(typeid(*d) == typeid(Device::Floppy)){
			Device::Floppy *f = dynamic_cast<Device::Floppy*>(d);
			f->Load(emu->memory, 0x7c00, 256);
		}else{
			throw "unknown bootable device.";
		}
	}
private:
	::x86::Emulator *emu;
};

}
}

#endif
