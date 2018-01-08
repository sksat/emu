#ifndef MODRM_H_
#define MODRM_H_

#include <sstream>
#include <stdint.h>
#include "emulator.h"

#define MOD	modrm.mod
#define RM	modrm.rm

namespace x86 {

class InsnData {
private:
	x86::Emulator *emu;
public:
	struct ModRM {
		uint8_t rm  : 3;
		uint8_t reg : 3;
		uint8_t mod : 2;
	};

	struct SIB {
		uint8_t base  : 3;
		uint8_t index : 3;
		uint8_t scale : 2;
	};

	uint8_t prefix;
	uint8_t opcode;

	union {
		uint8_t _modrm;
		struct ModRM modrm;
	};
	
	union {
		uint8_t reg;
		uint8_t subopcode;
	};

	union {
		uint8_t _sib;
		struct SIB sib;
	};

	union {
		int8_t  disp8;
		int16_t disp16;
		int32_t disp32;
	};

	union {
		int8_t  imm8;
		int16_t imm16;
		int32_t imm32;
	};
public:
	InsnData(x86::Emulator *e);

	inline void ParseModRM16(){
		switch(MOD){
		case 0b00:
			if(RM == 0b110)
				goto get_disp16;
			else
				goto not_impl;
		case 0b01:
			goto get_disp8;
		case 0b10:
			goto get_disp16;
		case 0b11:
			goto not_impl;
get_disp8:
	disp8 = static_cast<int8_t>(emu->GetCode8(0));
	emu->EIP++;
	DOUT("disp8");
	break;
get_disp16:
	disp16 = static_cast<int16_t>(emu->GetCode32(0));
	emu->EIP+=2;
	DOUT("disp16");
	break;
not_impl:
			{
				std::stringstream ss;
				ss << "not implemented: Mod=0x" << std::hex
					<< static_cast<uint32_t>(MOD)
					<< ", R/M="
					<< static_cast<uint32_t>(RM)
					<< std::endl;
				throw ss.str();
			}
		}
		DOUT(": 0x"<<std::hex<<disp16<<std::endl);
	}

	inline void ParseModRM32(){
		// SIB
		if(MOD != 0b11 && RM == 0b100){
			_sib = emu->GetCode8(0);
			emu->EIP++;
			DOUT("SIB: scale=0x"<<std::hex
					<< static_cast<uint32_t>(sib.scale)
					<< ", index=0x"
					<< static_cast<uint32_t>(sib.index)
					<< ", base=0x"
					<< static_cast<uint32_t>(sib.base)
					<< std::endl);
		}

		switch(MOD){
		case 0b00:
			if(RM == 0b101)
				goto get_disp32;
			else
				break;
		case 0b01:
			goto get_disp8;
		case 0b10:
			goto get_disp32;
		case 0b11:
			goto not_impl;
get_disp8:
	disp8 = static_cast<int8_t>(emu->GetCode8(0));
	emu->EIP++;
	DOUT("disp8");
	break;
get_disp32:
	disp32 = emu->GetSignCode32(0);
	emu->EIP+=4;
	DOUT("disp32");
	break;
not_impl:
			{
				std::stringstream ss;
				ss << "not implemented: Mod=0x" << std::hex
					<< static_cast<uint32_t>(MOD)
					<< ", R/M="
					<< static_cast<uint32_t>(RM)
					<< std::endl;
				throw ss.str();
			}
		}
		DOUT(": 0x"<<std::hex<<disp32<<std::endl);
	}

	inline uint32_t CalcMemAddr(){
		switch(modrm.mod){
			case 0:
				switch(modrm.rm){
					case 4:
						throw "not implemented ModRM mod=0, rm=4";
					case 5:
						return disp32;
					default:
						return emu->reg[modrm.rm];
				}
				break;
			case 1:
				if(modrm.rm == 4){
					throw "not implemented ModRM mod=1, rm=4";
				}else{
					return emu->reg[modrm.rm] + disp8;
				}
				break;
			case 2:
				if(modrm.rm == 4){
					throw "not implemented ModRM mod=2, rm=4";
				}else{
					return emu->reg[modrm.rm] + disp32;
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
		if(modrm.mod == 3)
			return emu->reg[modrm.rm];
		DOUT("GetRM32: mem addr ="<<std::hex<<CalcMemAddr()<<std::endl);
		return emu->memory->GetData32(CalcMemAddr());
	}
	inline void SetRM32(uint32_t val){
		if(modrm.mod == 3){
			emu->reg[modrm.rm] = val;
			DOUT("SetRM32: reg="<<emu->reg[modrm.rm].GetName()<<" val="<<std::hex<<val<<std::endl);
		}else{
			uint32_t addr = CalcMemAddr();
			emu->memory->SetData32(addr, val);
			DOUT("SetRM32: addr="<<std::hex<<addr<<" val="<<val<<std::endl);
		}
	}

};

};

#endif
