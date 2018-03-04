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
	uint8_t subopcode;

	union {
		uint8_t _modrm;
		struct ModRM modrm;
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

	uint32_t moffs;

	bool chsiz_op;		// オペランドサイズ変更
	bool chsiz_addr;	// アドレスサイズ変更

	SRegister *sreg = &DS;
public:
	InsnData(x86::Emulator *e);

	inline bool IsMode16(){
		return (emu->IsReal() ^ chsiz_op);
	}

	inline bool IsMode32(){
		return !IsMode16();
	}

	inline void ParseModRM16(){
		DOUT("ModRM16: ");
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
			EIP++;
			DOUT("disp8=0x"<<std::hex<<static_cast<uint32_t>(disp8));
			break;
get_disp16:
			disp16 = static_cast<int16_t>(emu->GetCode32(0));
			EIP+=2;
			DOUT("disp16=0x"<<std::hex<<disp16);
			break;
		}
	}

	inline void ParseModRM32(){
		DOUT(" ModRM32: ");
		// SIB
		if(MOD != 0b11 && RM == 0b100){
			_sib = emu->GetCode8(0);
			EIP++;
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
			DOUT("reg"<<std::endl);
			return;//break;
		case 0b01:
			goto get_disp8;
		case 0b10:
			goto get_disp32;
		case 0b11:
			break;
get_disp8:
			disp8 = static_cast<int8_t>(emu->GetCode8(0));
			EIP++;
			DOUT("disp8: 0x"<<std::hex<<static_cast<uint32_t>(disp8));
			break;
get_disp32:
			disp32 = emu->GetSignCode32(0);
			EIP+=4;
			DOUT("disp32: 0x"<<std::hex<<disp32);
			break;
		}
	}

	inline uint32_t CalcMemAddr(){
		if(IsMode32() ^ chsiz_addr)
			return CalcMemAddr32();
		else
			return CalcMemAddr16();
	}
	inline uint32_t CalcMemAddr16(){ // p35
		uint32_t addr = 0x00;
		switch(MOD){
		case 0b00:
			if(RM == 0b110){
				uint16_t tmp16 = disp16;
				addr = tmp16;
				DOUT(std::endl<<__func__<<": 0x"<<std::hex<<addr<<std::endl);
			//	getchar();
				return addr;
			}
			break;
		case 0b01:
			addr = static_cast<uint32_t>(disp8);
			break;
		case 0b10:
			addr = static_cast<uint32_t>(disp16);
			break;
		}

		switch(RM){
		case 0b000:
		case 0b001:
		case 0b111:
			addr += static_cast<uint32_t>(BX);
			break;
		case 0b010:
		case 0b011:
		case 0b110:
			addr += static_cast<uint32_t>(BP);
			std::cerr<<std::endl<<"TODO: not implemented: sreg=SS"<<std::endl;
			// TODO: segment register: SS
			break;
		}

		if(RM < 0b110){
			if(RM%2 == 0b00)
				addr += static_cast<uint32_t>(SI);
			else
				addr += static_cast<uint32_t>(DI);
		}

		return addr;
	}
	inline uint32_t CalcMemAddr32(){
		switch(MOD){
			case 0b00:
				switch(RM){
					case 0b100: // SIB
						break;
					case 0b101:
						return disp32;
					default:
						return emu->reg[RM].reg32;
				}
			case 0b01:
				if(RM == 0b100) // SIB
					break;
				else
					return emu->reg[RM].reg32 + disp8;
			case 0b10:
				if(RM == 0b100) // SIB
					break;
				else
					return emu->reg[RM].reg32 + disp32;
			case 0b11:
				break;
		}
		std::stringstream ss;
		ss << "not implemented ModRM: Mod="<<static_cast<uint32_t>(MOD)
			<< ", R/M="<<static_cast<uint32_t>(RM);
		throw ss.str();
	}

	// get register
	inline uint8_t GetR8(){
		return GET_REG8(modrm.reg);
	}
	inline uint16_t GetR16(){
		return emu->reg[modrm.reg].reg16;
	}
	inline uint32_t GetR32(){
		return emu->reg[modrm.reg].reg32;
	}

	// set register
	inline void SetR8(uint8_t val){
		SET_REG8(modrm.reg, val);
	}
	inline void SetR16(uint16_t val){
		emu->reg[modrm.reg].reg16 = val;
	}
	inline void SetR32(uint32_t val){
		emu->reg[modrm.reg].reg32 = val;
	}

	// get rm
	inline uint8_t GetRM8(){
		if(MOD == 3)
			return GET_REG8(RM);
		auto addr = CalcMemAddr();
//		DOUT("GetRM8: addr=0x"<<std::hex<<addr);
		return GET_DATA8(addr); //emu->memory->GetData8(addr);
	}
	inline uint16_t GetRM16(){
		if(MOD == 3)
			return emu->reg[RM].reg16;
		auto addr = CalcMemAddr();
//		DOUT("GetRM16: addr=0x"<<std::hex<<addr);
		return GET_DATA16(addr); //emu->memory->GetData16(addr);
	}
	inline uint32_t GetRM32(){
		if(MOD == 3){
			auto& reg = emu->reg[RM];
//			DOUT("GetRM32: reg:"<<reg.GetName()<<"("<<std::hex<<reg.reg32<<")");
			return reg.reg32;
		}
		auto addr = CalcMemAddr();
//		DOUT("GetRM32: addr=0x"<<std::hex<<addr);
		return GET_DATA32(addr); //emu->memory->GetData32(addr);
	}

	// set rm
	inline void SetRM8(uint8_t val){
		if(MOD == 3){
			SET_REG8(RM, val);
			DOUT("SetRM8: "<<emu->reg[RM].GetName()
					<<" = 0x"<<std::hex<<static_cast<uint32_t>(val)<<std::endl);
			return;
		}
		auto addr = CalcMemAddr();
		SET_DATA8(addr, val); //emu->memory->SetData8(addr, val);
		DOUT("SetRM8: [0x"<<std::hex<<addr
				<<"] = 0x"<<static_cast<uint32_t>(val)<<std::endl);
	}
	inline void SetRM16(uint16_t val){
		if(MOD == 3){
			emu->reg[RM].reg16 = val;
			DOUT("SetRM16: "<<emu->reg[RM].GetName()<<" = 0x"<<std::hex<<val<<std::endl);
			return;
		}
		auto addr = CalcMemAddr();
		SET_DATA16(addr, val); //emu->memory->SetData16(addr, val);
		DOUT("SetRM16: [0x"<<std::hex<<addr
				<<"] = 0x"<<val<<std::endl);
	}
	inline void SetRM32(uint32_t val){
		if(modrm.mod == 3){
			emu->reg[RM].reg32 = val;
			DOUT("SetRM32: reg="<<emu->reg[RM].GetName()<<" val="<<std::hex<<val<<std::endl);
			return;
		}
		uint32_t addr = CalcMemAddr();
		SET_DATA32(addr, val); //emu->memory->SetData32(addr, val);
		DOUT("SetRM32: [0x"<<std::hex<<addr
				<<"] = 0x"<<val<<std::endl);
	}

};

};

#endif
