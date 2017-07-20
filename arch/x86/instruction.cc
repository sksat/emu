#include <sstream>
#include "instruction.h"
#include "emulator.h"

namespace x86 {

Instruction::Instruction(x86::Emulator *e) : emu(e) {
	modrm = new ModRM(e, 0x00);
}

void Instruction::Init(){
	// default insn
	ClearInsn(0xff);
	opcode = 0x90;

	SETINSN(0x90, nop,			0);
	SETINSN(0xe9, near_jump,	0);
	SETINSN(0xeb, short_jump,	0);
}

void Instruction::Parse(){
	prefix = opcode = (*emu->memory)[(uint32_t)emu->reg[8]];
	std::stringstream ss;
	switch(prefix){
		case 0xf0:
		case 0xf2:
		case 0xf3:
		case 0x26:
		case 0x2e:
		case 0x36:
		case 0x3e:
		case 0x64:
		case 0x65:
		case 0x66:
		case 0x67:
			ss<<"not implemented prefix:"<<std::hex<<std::showbase<<(uint32_t)prefix<<std::endl;
			throw ss.str();
			break;
		default:
			break;
	}
	//if modrm
	if(insn_flgs[opcode]){
		emu->EIP++;
		modrm->Set(emu->GetCode8(0));
		modrm->Parse();
		emu->EIP++;
		if(modrm->IsSIB()){
			sib = emu->GetCode8(0);
			emu->EIP++;
		}
		if(modrm->IsDisp32()){
			disp32 = emu->GetSignCode32(0);
			emu->EIP+=4;
		}else if(modrm->GetMod() == 1){
			disp8 = emu->GetSignCode8(0);
			emu->EIP++;
		}
	}
}

void Instruction::ExecStep(){
	Parse();
	insnfunc_t func = insn[opcode];
	(this->*func)();
	if(emu->EIP == 0) emu->finish_flg=true;
}

void Instruction::not_impl_insn(){
	std::stringstream ss;
	ss<<"x86: not implemented insn : "<<std::hex<<std::showbase<<static_cast<uint32_t>(opcode);
	throw ss.str();
}

};
