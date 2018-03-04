#ifndef X86_INSTRUCTION32_H_
#define X86_INSTRUCTION32_H_
#include <sstream>
#include "instruction.h"

namespace x86 {

class Instruction32 : public x86::Instruction {
public:
	Instruction32(x86::Emulator *e, InsnData *i) : Instruction(e, i) {}
	void Init();
private:
	void add_rm32_r32(){
		idata->SetRM32(idata->GetRM32() + emu->reg[idata->modrm.reg].reg32);
	}

	void and_eax_imm32(){
		DOUT(std::endl<<"EAX <- EAX(0x"<<std::hex<<EAX<<") & "<<idata->imm32);
		EAX = EAX & idata->imm32;
		DOUT(" = 0x"<<EAX);
	}

	void cmp_r32_rm32(){
		uint32_t r32  = emu->reg[idata->modrm.reg].reg32;
		uint32_t rm32 = idata->GetRM32();
		uint64_t res  = (uint64_t)r32 - (uint64_t)rm32;
		emu->eflags.UpdateSub(r32, rm32, res);
	}
	void inc_r32(){
		uint8_t r = emu->GetCode8(0) - 0x40;
		emu->reg[r].reg32++;
		EIP++;
	}
	void push_r32(){
		emu->push32(emu->reg[emu->GetCode8(0)-0x50].reg32);
		EIP++;
	}
	void pop_r32(){
		emu->reg[emu->GetCode8(0) - 0x58].reg32 = emu->pop32();
		EIP++;
	}

	void imul_r32_rm32_imm32(){
		auto& reg = emu->reg[idata->modrm.reg];
		uint32_t rm32 = idata->GetRM32();
		uint64_t temp = static_cast<int32_t>(rm32) * idata->imm32;
		DOUT(std::endl<<reg.GetName()<<" <- 0x"<<std::hex<<rm32<<" * 0x"<<idata->imm32<<" = 0x"<<temp);
		reg.reg32 = static_cast<uint32_t>(temp);
		if(temp != reg.reg32) // temp != DEST
			EFLAGS.CF = EFLAGS.OF = 1;
		else
			EFLAGS.CF = EFLAGS.OF = 0;
	}

	void sub_rm32_imm32(){
		uint32_t rm32 = idata->GetRM32();
		int32_t set = static_cast<int32_t>(rm32) - idata->imm32;
		DOUT(std::endl<<"neko: RM32=0x"<<std::hex<<static_cast<int32_t>(rm32)<<", imm32=0x"<<idata->imm32<<", set=0x"<<set);
		idata->SetRM32(set);
		EFLAGS.UpdateSub(rm32, idata->imm32, set);
	}

	void code_83(){
		switch(idata->modrm.reg){
		case 0:
			add_rm32_imm8();
			break;
		case 1:
			or_rm32_imm8();
			break;
		case 5:
			sub_rm32_imm8();
			break;
		default:
			std::stringstream ss;
			ss<<"not implemented: 83 "<<std::hex<<(uint32_t)idata->modrm.reg;
			throw ss.str();
		}

	}
		void add_rm32_imm8(){
			uint32_t rm32 = idata->GetRM32();
			uint64_t set = rm32 + idata->imm8;
			idata->SetRM32(set);
			EFLAGS.UpdateAdd(rm32, idata->imm8, set);
		}
		void or_rm32_imm8(){
			uint32_t rm32 = idata->GetRM32();
			uint64_t set = rm32 | idata->imm8;
			idata->SetRM32(set);
			EFLAGS.UpdateOr(rm32, idata->imm8, set);
		}
		void sub_rm32_imm8(){
			uint32_t rm32 = idata->GetRM32();
			uint64_t set = rm32 - idata->imm8;
			idata->SetRM32(set);
			EFLAGS.UpdateSub(rm32, idata->imm8, set);
		}
	void mov_rm32_r32(){
		idata->SetRM32(emu->reg[idata->modrm.reg].reg32);
	}
	void mov_r32_rm32(){
		auto& reg = emu->reg[idata->modrm.reg];
		DOUT(__func__<<": "<<reg.GetName()<<" <- 0x"<<std::hex<<idata->GetRM32()<<std::endl);
		reg.reg32 = idata->GetRM32();
	}
	void mov_r32_imm32(){
		auto& reg = emu->reg[idata->opcode - 0xb8];
		DOUT(__func__<<": "<<reg.GetName()<<" <- "<<std::hex<<idata->imm32);
		reg.reg32 = idata->imm32;
//		emu->reg[idata->opcode - 0xB8].reg32 = idata->imm32;
	}

	void code_c1(){
		switch(idata->modrm.reg){
		case 5:
			shr_rm16_imm8();
			break;
		default:
			std::stringstream ss;
			ss << "not implemented: 0xc1 /"<<std::dec<<static_cast<uint32_t>(idata->modrm.reg);
			throw ss.str();
		}
	}
		void shr_rm16_imm8(){
			uint16_t rm16 = idata->GetRM16();
			uint32_t result = rm16 >> idata->imm8;
			idata->SetRM16(result);
			EFLAGS.UpdateShr(rm16, idata->imm8, result);
		}

	void ret32(){
		EIP = emu->pop32();
	}
	void mov_rm32_imm32(){
//		uint32_t val = emu->GetCode32(0);
		DOUT("mov_rm32_imm32: val="<<idata->imm32<<std::endl);
//		EIP += 4;
		idata->SetRM32(idata->imm32);
//		idata->SetRM32(emu->GetCode32(-4));
	}
	void leave32(){
		ESP = EBP;
		EBP = emu->pop32();
		EIP++;
	}
	void call_rel32(){
		int32_t diff = emu->GetSignCode32(1);
		emu->push32(EIP + 5);
		EIP += (diff + 5);
	}
	void code_ff(){
		switch(idata->modrm.reg){
		case 0:
			inc_rm32();
			break;
		default:
			throw "not implemented: FF "+std::to_string((int)idata->opcode);
		}
	}
		void inc_rm32(){
			idata->SetRM32(idata->GetRM32() + 1);
		}
};

};

#endif
