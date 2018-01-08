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
				break;
		case 0b01:
			goto get_disp8;
		case 0b10:
			goto get_disp16;
		case 0b11:
			break;
get_disp8:
	disp8 = static_cast<int8_t>(emu->GetCode8(0));
	emu->EIP++;
	DOUT("disp8: 0x"<<std::hex<<static_cast<uint32_t>(disp8)<<std::endl);
	break;
get_disp16:
	disp16 = static_cast<int16_t>(emu->GetCode32(0));
	emu->EIP+=2;
	DOUT("disp16: 0x"<<std::hex<<disp16<<std::endl);
	break;
		}
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
			break;
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
		}
		DOUT(": 0x"<<std::hex<<disp32<<std::endl);
	}

	inline uint32_t CalcMemAddr(){
		if(emu->IsMode16())
			return CalcMemAddr16();
		else
			return CalcMemAddr32();
	}
	inline uint32_t CalcMemAddr16(){
		throw "not implemented: CalcMemAddr16";
	}
	inline uint32_t CalcMemAddr32(){
		switch(MOD){
			case 0:
				switch(RM){
					case 4:
						throw "not implemented ModRM mod=0, rm=4";
					case 5:
						return disp32;
					default:
						return emu->reg[RM];
				}
				break;
			case 1:
				if(RM == 4){
					throw "not implemented ModRM mod=1, rm=4";
				}else{
					return emu->reg[RM] + disp8;
				}
				break;
			case 2:
				if(RM == 4){
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

	// get register
	inline uint8_t GetR8(){
		return emu->reg[modrm.reg];
	}
	inline uint16_t GetR16(){
		return emu->reg[modrm.reg];
	}
	inline uint32_t GetR32(){
		return emu->reg[modrm.reg];
	}

	// set register
	inline void SetR8(uint8_t val){
		emu->reg[modrm.reg] = val;
	}
	inline void SetR16(uint16_t val){
		emu->reg[modrm.reg] = val;
	}
	inline void SetR32(uint32_t val){
		emu->reg[modrm.reg] = val;
	}

	// get rm
	inline uint8_t GetRM8(){
		if(MOD == 3)
			return emu->reg[RM];
		auto addr = CalcMemAddr();
		DOUT("GetRM8: addr=0x"<<std::hex<<addr<<std::endl);
		return emu->memory->GetData8(addr);
	}
	inline uint16_t GetRM16(){
		if(MOD == 3)
			return emu->reg[RM];
		auto addr = CalcMemAddr();
		DOUT("GetRM16: addr=0x"<<std::hex<<addr<<std::endl);
//		return emu->memory->GetData16(addr);
		throw "not implemented: memory::GetData16";
	}
	inline uint32_t GetRM32(){
		if(MOD == 3)
			return emu->reg[modrm.rm];
		auto addr = CalcMemAddr();
		DOUT("GetRM32: addr=0x"<<std::hex<<addr<<std::endl);
		return emu->memory->GetData32(addr);
	}

	// set rm
	inline void SetRM8(uint8_t val){
		if(MOD == 3){
			emu->reg[RM] = val;
			DOUT("SetRM8: "<<emu->reg[RM].GetName()
					<<" = 0x"<<std::hex<<static_cast<uint32_t>(val)<<std::endl);
			return;
		}
		auto addr = CalcMemAddr();
		emu->memory->SetData8(addr, val);
		DOUT("SetRM8: [0x"<<std::hex<<addr
				<<"] = 0x"<<static_cast<uint32_t>(val)<<std::endl);
	}
	inline void SetRM16(uint16_t val){
		if(MOD == 3){
			emu->reg[RM] = val;
			DOUT("SetRM16: "<<emu->reg[RM].GetName()<<" = 0x"<<std::hex<<val<<std::endl);
			return;
		}
		auto addr = CalcMemAddr();
//		emu->memory->SetData16(addr, val);
		DOUT("SetRM16: [0x"<<std::hex<<addr
				<<"] = 0x"<<val<<std::endl);
		throw "not implemented: memory::SetData16";
	}
	inline void SetRM32(uint32_t val){
		if(modrm.mod == 3){
			emu->reg[RM] = val;
			DOUT("SetRM32: reg="<<emu->reg[RM].GetName()<<" val="<<std::hex<<val<<std::endl);
			return;
		}
		uint32_t addr = CalcMemAddr();
		emu->memory->SetData32(addr, val);
		DOUT("SetRM32: [0x"<<std::hex<<addr
				<<"] = 0x"<<val<<std::endl);
	}

};

};

#endif
