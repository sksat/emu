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
	void inc_r32(){
		uint8_t r = emu->GetCode8(0) - 0x40;
		emu->reg[r].reg32++;
		emu->EIP++;
	}
	void mov_rm32_imm32(){
		uint32_t val = emu->GetCode32(0);
		std::cout<<"mov_rm32_imm32: val="<<val<<std::endl;
		emu->EIP += 4;
		idata->SetRM32(val);
//		idata->SetRM32(emu->GetCode32(-4));
	}
};

};

