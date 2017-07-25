#include <sstream>
#include "instruction.h"
#include "emulator.h"

namespace x86 {

Instruction::Instruction(x86::Emulator *e) : emu(e) {
	idata = new InsnData(e);
}

void Instruction::Init(){
	// default insn
	ClearInsn(256);
	idata->opcode = 0x90;

	SETINSN(0x90, nop,			0);
	SETINSN(0xe9, near_jump,	0);
	SETINSN(0xeb, short_jump,	0);
}

void Instruction::Parse(){
	idata->prefix = idata->opcode = (*emu->memory)[emu->reg[8].reg32];
	std::stringstream ss;
	switch(idata->prefix){
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
			ss<<"not implemented prefix:"<<std::hex<<std::showbase<<(uint32_t)idata->prefix<<std::endl;
			throw ss.str();
			break;
		default:
			break;
	}
	//if modrm
	if(insn_flgs[idata->opcode]){
		std::cout<<"ModRM : ";
		emu->EIP++;
		idata->SetModRM(emu->GetCode8(0));
		std::cout<<"Mod="<<std::hex<<(uint32_t)idata->mod
				<<" RM="<<(uint32_t)idata->rm<<std::endl;
		emu->EIP++;
		if(idata->IsSIB()){
			idata->sib = emu->GetCode8(0);
			emu->EIP++;
		}
		if(idata->IsDisp32()){
			idata->disp32 = emu->GetSignCode32(0);
			emu->EIP+=4;
		}else if(idata->mod == 1){
			idata->disp8 = emu->GetSignCode8(0);
			emu->EIP++;
		}
	}
}

void Instruction::ExecStep(){
	Parse();
	std::cout<<"opcode = "<<std::hex<<(uint32_t)idata->opcode<<std::endl;
	insnfunc_t func = insn[idata->opcode];
	(this->*func)();
	if(emu->EIP == 0) emu->finish_flg=true;
}

void Instruction::not_impl_insn(){
	std::stringstream ss;
	ss<<"x86: not implemented insn : "<<std::hex<<std::showbase<<static_cast<uint32_t>(idata->opcode);
	throw ss.str();
}

};
