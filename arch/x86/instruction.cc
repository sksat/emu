#include <sstream>
#include "instruction.h"
#include "emulator.h"

#define SETINSN(op,func,insn_flg) {insn[op] = (insnfunc_t)&x86::Instruction::func; insn_name[op] = #func; insn_flgs[op] = insn_flg;}

namespace x86 {

Instruction::Instruction(x86::Emulator *e) : emu(e) {
	idata = new InsnData(e);
	insn_name.resize(256);
}

void Instruction::Init(){
	// default insn
	ClearInsn(256);
	idata->opcode = 0x90;

//	SETINSN(0x00, add_rm8_r8,		Flag::ModRM);
	SETINSN(0x70, jo,			0);
	SETINSN(0x71, jno,			0);
	SETINSN(0x72, jc,			0);	// = jb
	SETINSN(0x73, jnc,			0); // = jnb
	SETINSN(0x74, jz,			0);
	SETINSN(0x75, jnz,			0);
//	SETINSN(0x76, jbe,			0);
//	SETINSN(0x77, ja,			0);
	SETINSN(0x78, js,			0);
	SETINSN(0x79, jns,			0);
//	SETINSN(0x7a, jp,			0);
//	SETINSN(0x7b, jnp,			0);
//	SETINSN(0x7c, jl,			0);
//	SETINSN(0x7d, jnl,			0);
//	SETINSN(0x7e, jle,			0);
//	SETINSN(0x7f, jnle,			0);
	SETINSN(0x90, nop,			0);
	SETINSN(0xe9, near_jump,	0);
	SETINSN(0xeb, short_jump,	0);
}

void Instruction::Parse(){
	idata->prefix = idata->opcode = emu->GetCode8(0);

	// parse prefix
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
		{
			std::stringstream ss;
			ss << "not implemented prefix:"
				<< std::hex << std::showbase
				<< (uint32_t)idata->prefix
				<<std::endl;
			throw ss.str();
		}
			break;
		default:
			idata->prefix = 0x00;
			EIP++;
			break;
	}

	auto& flgs = insn_flgs[idata->opcode];
	//if ModR/M
	if(flgs & Flag::ModRM){
		idata->_modrm = emu->GetCode8(0);
		DOUT("ModRM: Mod=0x" << std::hex
				<< (uint32_t)idata->modrm.mod
				<< " RM=0x"
				<< (uint32_t)idata->modrm.rm
				<< std::endl);
		EIP++;
		if(emu->IsMode16()) // 16bit mode
			idata->ParseModRM16();
		else // 32bit mode
			idata->ParseModRM32();
	}

	// imm
	if(flgs & Flag::Imm8){
		idata->imm8 = emu->GetSignCode8(0);
		EIP++;
		DOUT("imm8 : "<<std::hex<<static_cast<uint32_t>(idata->imm8)<<std::endl);
	}
	if(flgs & Flag::Imm16){
		idata->imm16 = emu->GetSignCode16(0);
		EIP+=2;
		DOUT("imm16: "<<std::hex<<idata->imm16<<std::endl);
	}
	if(flgs % Flag::Imm32){
		idata->imm32 = emu->GetSignCode32(0);
		EIP+=4;
		DOUT("imm32: "<<std::hex<<idata->imm32<<std::endl);
	}

}

void Instruction::ExecStep(){
	Parse();
	DOUT("opcode = 0x"
		<< std::setw(2) << std::setfill('0')<< std::hex
		<< (uint32_t)idata->opcode
		<< ": " << insn_name[idata->opcode]
		<<std::endl
	);
	insnfunc_t func = insn[idata->opcode];
	(this->*func)();
	if(EIP == 0x00) emu->finish_flg=true;
}

void Instruction::not_impl_insn(){
	std::stringstream ss;
	ss << "x86: not implemented insn : 0x" 
		<< std::setw(2) << std::setfill('0') << std::hex
		<< static_cast<uint32_t>(idata->opcode);
	throw ss.str();
}

};
