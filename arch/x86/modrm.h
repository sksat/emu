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
	union {
		int8_t disp8;
		uint32_t disp32;
	};
public:
	ModRM(x86::Emulator *e);
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
	inline uint32_t CalcMemAddr(){
		switch(mod){
			case 0:
				switch(rm){
					case 4:
						throw "not implemented ModRM mod=0, rm=4";
					case 5:
						return disp32;
					default:
						return emu->reg[rm];
				}
				break;
			case 1:
				if(rm == 4){
					throw "not implemented ModRM mod=1, rm=4";
				}else{
					return emu->reg[rm].reg32 + disp8;
				}
				break;
			case 2:
				if(rm == 4){
					throw "not implemented ModRM mod=2, rm=4";
				}else{
					return emu->reg[rm].reg32 + disp32;
				}
				break;
			case 3:
				throw "not implemented ModRM mod=3";
				break;
			default:
				throw "ModRM error";
				break;
		}
	}
	inline void SetRM32(uint32_t val){
		if(mod == 3){
			emu->reg[rm].reg32 = val;
		}else{
			emu->memory[CalcMemAddr()] = val;
		}
	}
};

};

#endif
