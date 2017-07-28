#ifndef MODRM_H_
#define MODRM_H_

#include <sstream>
#include <stdint.h>
#include "emulator.h"

namespace x86 {

class InsnData {
private:
	x86::Emulator *emu;
	uint8_t _modrm;
public:
	uint8_t prefix, opcode;
	uint8_t mod,rm;
	union {
		uint8_t reg;
		uint8_t subopcode;
	};
	uint8_t sib;
	union {
		int8_t disp8;
		uint32_t disp32;
	};
public:
	InsnData(x86::Emulator *e);
	inline InsnData& operator=(uint8_t code){
		_modrm = code;
		return *this;
	}
	inline void SetModRM(uint8_t code){
		mod = ((code & 0xC0) >> 6);
		reg = ((code & 0x38) >> 3);
		rm  = code & 0x07;
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
					return emu->reg[rm] + disp8;
				}
				break;
			case 2:
				if(rm == 4){
					throw "not implemented ModRM mod=2, rm=4";
				}else{
					return emu->reg[rm] + disp32;
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
	inline uint32_t GetRM32(){
		if(mod == 3)
			return emu->reg[rm];
std::cout<<"GetRM32: mem addr ="<<std::hex<<CalcMemAddr()<<std::endl;
		return emu->memory->GetData32(CalcMemAddr());
	}
	inline void SetRM32(uint32_t val){
		if(mod == 3){
			emu->reg[rm] = val;
			std::cout<<"SetRM32: reg="<<emu->reg[rm].GetName()<<" val="<<std::hex<<val<<std::endl;
		}else{
			uint32_t addr = CalcMemAddr();
			emu->memory->SetData32(addr, val);
			std::cout<<"SetRM32: addr="<<std::hex<<addr<<" val="<<val<<std::endl;
		}
	}
};

};

#endif
