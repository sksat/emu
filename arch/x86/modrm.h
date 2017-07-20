#ifndef MODRM_H_
#define MODRM_H_

#include <stdint.h>
#include "emulator.h"

namespace x86 {

class ModRM {
private:
	x86::Emulator *emu;
	uint8_t mod,rm,reg;
	uint8_t _modrm;
public:
	ModRM(x86::Emulator *e);
	ModRM(x86::Emulator *e, uint8_t modrm);
	inline ModRM& operator=(uint8_t code){
		_modrm = code;
		return *this;
	}
	inline void Set(uint8_t code){
		_modrm = code;
	}
	inline uint8_t GetMod(){	return mod; }
	inline uint8_t GetRM(){		return rm; }
	inline void Parse(){
		mod = ((_modrm & 0xC0) >> 6);
		reg = ((_modrm & 0x38) >> 3);
		rm  = _modrm & 0x70;
		emu->EIP++;
	}
	inline bool IsSIB(){
		if(mod != 3 && rm == 4)
			return true;
		return false;
	}
	inline bool IsDisp32(){
		if((mod == 0 && rm == 5) || mod == 2)
			return true;
		return false;
	}
};

};

#endif
