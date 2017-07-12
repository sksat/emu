#include "instruction.h"

#undef SETINSN
#define SETINSN(o,f) (insn[o] = (insnfunc_t)&Instruction32::f)

namespace x86 {

class Instruction32 : public x86::Instruction {
public:
	Instruction32(x86::Emulator *e) : Instruction(e) {}
	void Init();
private:
	void inc_r32(){
		uint8_t r = emu->GetCode8(0) - 0x40;
		emu->reg[r].reg32++;
		emu->EIP++;
	}
};

};

