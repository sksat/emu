#include "instruction.h"

namespace osecpu {

void Instruction::Init(){
	throw "osecpu::Instruction::Init() is not implemented.";
	ClearInsn(0xff);
	opcode = 0x00;

	SETINSN(0x00, nop);
}

void Instruction::Parse(){
	throw "osecpu::Instruction::Parse() is not implemented.";
}

void Instruction::ExecStep(){
	throw "osecpu::Instruction::ExecStep() is not implemented.";
}

};
