#ifndef BIOS_JUNK_X86_H_
#define BIOS_JUNK_X86_H_

#include <iostream>
#include <typeinfo>
#include "junk_base.h"
#include "../arch/x86/emulator.h"
#include "../device/floppy.h"
#include "../device/display.h"

// http://oswiki.osask.jp/?(AT)BIOS

namespace BIOS {
namespace Junk {

class x86 : public BIOS::Junk::Base {
public:
	x86(EmulatorBase *e) : BIOS::Junk::Base(e) {
		if(typeid(*e) != typeid(::x86::Emulator))
			throw "emulator is not for x86";
		emu = dynamic_cast<::x86::Emulator*>(e);
	}

	void SetDisplay(Device::Display &disp){ this->disp = &disp; }

	void Boot() {
		using namespace std;
		cout<<"booting x86 BIOS..."<<endl;

		LoadIPL();

		EIP = 0x7c00;
		ESP = 0x7c00;
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

	void Function(size_t num){
		DOUT("\n\tBIOS function called: num = 0x"<<std::hex<<num<<std::endl);
		switch(num){
		case 0x10:
			video_func();
			break;
		default:
			throw "not implemented: BIOS function";
			break;
		}
	}

	void video_func(){
		DOUT("\tvideo function  ");
		DOUT("AH = 0x"<<std::hex<<static_cast<uint32_t>(AH)<<std::endl);
		switch(AH){
		case 0x00:
			std::cerr<<"warning: not implemented video mode"<<std::endl;
			break;
		case 0x0e:
			std::cout
#ifdef DEBUG
			<<"BIOS function putchar: "
#endif
			<<AL
#ifdef DEBUG
			<<std::endl
#endif
			;
			if(disp == nullptr) throw "BIOS: Display is null";
			disp->Print(static_cast<char>(AL));
			break;
		default:
			throw "not implemented: video_func(junk BIOS)";
			break;
		}
	}
private:
	::x86::Emulator *emu;
	Device::Display *disp;
};

}
}

#endif
