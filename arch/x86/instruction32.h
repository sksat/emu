#ifndef X86_INSTRUCTION32_H_
#define X86_INSTRUCTION32_H_
#include <sstream>
#include "instruction.h"

namespace x86 {

class Instruction32 : public x86::Instruction {
public:
	Instruction32(x86::Emulator *e) : Instruction(e) {}
	void Init();
private:
	void add_rm32_r32(){
		idata->SetRM32(idata->GetRM32() + emu->reg[idata->modrm.reg].reg32);
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
	void code_83(){
		switch(idata->modrm.reg){
		case 0:
			add_rm32_imm8();
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
			uint32_t imm8 = (int32_t)emu->GetSignCode8(0);
			EIP++;
			idata->SetRM32(rm32 + imm8);
		}
		void sub_rm32_imm8(){
//			std::cout<<"sub"<<std::endl;
			uint32_t rm32 = idata->GetRM32();
			uint32_t imm8 = (int32_t)emu->GetSignCode8(0);
			EIP++;
			idata->SetRM32(rm32 - imm8);
			
//			emu->EIP++;
//			idata->SetRM32(idata->GetRM32() - (int32_t)emu->GetSignCode8(-1));
		}
	void mov_rm32_r32(){
		idata->SetRM32(emu->reg[idata->modrm.reg].reg32);
	}
	void mov_r32_rm32(){
		DOUT("mov_r32_rm32 "<<std::hex<<idata->GetRM32()<<std::endl);
		emu->reg[idata->modrm.reg].reg32 = idata->GetRM32();
	}
	void mov_r32_imm32(){
		emu->reg[emu->GetCode8(0) - 0xB8].reg32 = emu->GetCode32(1);
		EIP+=5;
	}
	void ret32(){
		EIP = emu->pop32();
	}
	void mov_rm32_imm32(){
		uint32_t val = emu->GetCode32(0);
		DOUT("mov_rm32_imm32: val="<<val<<std::endl);
		EIP += 4;
		idata->SetRM32(val);
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
