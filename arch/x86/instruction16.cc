#include "instruction16.h"

#define SETINSN(op,func,insn_flg) {insn[op] = (insnfunc_t)&x86::Instruction16::func; insn_flgs[op] = insn_flg;}

namespace x86 {

void Instruction16::Init(){
	Instruction::Init();
}

void Instruction16::Parse(){
	Instruction::Parse();
}

};
