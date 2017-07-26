#include "instruction.h"

#undef SETINSN
#define SETINSN(op,func,flg) {insn[op] = (insnfunc_t)&Instruction32::func;\
						insn_flgs[op] = flg;}

namespace x86 {

class Instruction32 : public x86::Instruction {
public:
	Instruction32(x86::Emulator *e) : Instruction(e) {}
	void Init();
	void Parse();
private:
	void add_rm32_r32(){
		idata->SetRM32(idata->GetRM32() + emu->reg[idata->reg].reg32);
	}
	void inc_r32(){
		uint8_t r = emu->GetCode8(0) - 0x40;
		emu->reg[r].reg32++;
		emu->EIP++;
	}
	void code_83(){
		switch(idata->subopcode){
		case 5:
			sub_rm32_imm8();
			break;
		default:
			throw "not implemented: 83 "+std::to_string((int)idata->opcode);
		}

	}
		void sub_rm32_imm8(){
			std::cout<<"sub"<<std::endl;
			uint32_t rm32 = idata->GetRM32();
			uint32_t imm8 = (int32_t)emu->GetSignCode8(0);
			emu->EIP++;
			idata->SetRM32(rm32 - imm8);
			
//			emu->EIP++;
//			idata->SetRM32(idata->GetRM32() - (int32_t)emu->GetSignCode8(-1));
		}
	void mov_rm32_r32(){
		idata->SetRM32(emu->reg[idata->reg].reg32);
	}
	void mov_r32_rm32(){
std::cout<<"mov_r32_rm32 "<<std::hex<<idata->GetRM32()<<std::endl;
		emu->reg[idata->reg].reg32 = idata->GetRM32();
	}
	void mov_r32_imm32(){
		emu->reg[emu->GetCode8(0) - 0xB8].reg32 = emu->GetCode32(1);
		emu->EIP+=5;
	}
	void ret32(){
		emu->EIP = emu->pop32();
	}
	void mov_rm32_imm32(){
		uint32_t val = emu->GetCode32(0);
		std::cout<<"mov_rm32_imm32: val="<<val<<std::endl;
		emu->EIP += 4;
		idata->SetRM32(val);
//		idata->SetRM32(emu->GetCode32(-4));
	}
	void call_rel32(){
		int32_t diff = emu->GetSignCode32(1);
		emu->push32(emu->EIP + 5);
		emu->EIP += (diff + 5);
	}
	void code_ff(){
		switch(idata->subopcode){
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

