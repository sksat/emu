#include "instruction.h"

namespace x86 {

void Instruction::Init(){
	insn = std::vector<insnfunc_t>(0xff, nullptr);
//	insn.push_back((insnfunc_t)&Instruction::nop);
}

void Instruction::Parse(){
	
}

void Instruction::ExecStep(){
	throw "not implemented insn.";
}

};
