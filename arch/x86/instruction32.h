#include "instruction.h"

#undef SETINSN
#define SETINSN(o,f) (insn[o] = (insnfunc_t)&Instruction32::f)

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
		emu->EIP += 4;
		modrm->SetRM32(emu->GetCode32(-4));
	}
};

};

