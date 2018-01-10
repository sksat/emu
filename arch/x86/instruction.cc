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
	SETINSN(0x70, jo,			Flag::Imm8);
	SETINSN(0x71, jno,			Flag::Imm8);
	SETINSN(0x72, jc,			Flag::Imm8);	// = jb
	SETINSN(0x73, jnc,			Flag::Imm8); // = jnb
	SETINSN(0x74, jz,			Flag::Imm8);
	SETINSN(0x75, jnz,			Flag::Imm8);
//	SETINSN(0x76, jbe,			0);
//	SETINSN(0x77, ja,			0);
	SETINSN(0x78, js,			Flag::Imm8);
	SETINSN(0x79, jns,			Flag::Imm8);
//	SETINSN(0x7a, jp,			0);
//	SETINSN(0x7b, jnp,			0);
//	SETINSN(0x7c, jl,			0);
//	SETINSN(0x7d, jnl,			0);
//	SETINSN(0x7e, jle,			0);
//	SETINSN(0x7f, jnle,			0);
	SETINSN(0x8a, mov_r8_rm8,		Flag::ModRM);
	SETINSN(0x8e, mov_sreg_rm16,		Flag::ModRM);
	SETINSN(0x90, nop,			Flag::None);
//	SETINSN(0xe9, near_jump,	0); // TODO: 32bitだったので32bitの方に移す
	SETINSN(0xeb, short_jump,		Flag::Imm8);
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

	DOUT("opcode = 0x"
		<< std::setw(2) << std::setfill('0') << std::hex
		<< (uint32_t)idata->opcode
		<< ": " << insn_name[idata->opcode]
		<< "\t"
	);

	auto& flgs = insn_flgs[idata->opcode];
	//if ModR/M
	if(flgs & Flag::ModRM){
		idata->_modrm = emu->GetCode8(0);
		DOUT("ModRM: Mod=0x" << std::hex
				<< (uint32_t)idata->modrm.mod
				<< " RM=0x"
				<< (uint32_t)idata->modrm.rm);
		EIP++;
		if(emu->IsMode16()) // 16bit mode
			idata->ParseModRM16();
		else // 32bit mode
			idata->ParseModRM32();
	}

	// imm
	if(flgs & Flag::Imm8){
		idata->imm8 = (int8_t)emu->GetCode8(0);
		EIP++;
		DOUT("  imm8=0x"<<std::hex<<static_cast<int32_t>(idata->imm8));
	}
	if(flgs & Flag::Imm16){
		idata->imm16 = emu->GetSignCode16(0);
		EIP+=2;
		DOUT(" imm16=0x"<<std::hex<<idata->imm16);
	}
	if(flgs & Flag::Imm32){
		idata->imm32 = emu->GetSignCode32(0);
		EIP+=4;
		DOUT(" imm32=0x"<<std::hex<<idata->imm32);
	}

}

void Instruction::ExecStep(){
	Parse();
	insnfunc_t func = insn[idata->opcode];
	(this->*func)();
	if(EIP == 0x00) emu->finish_flg=true;
	DOUT(std::endl);
}

void Instruction::not_impl_insn(){
	std::stringstream ss;
	ss << "x86: not implemented insn : 0x" 
		<< std::setw(2) << std::setfill('0') << std::hex
		<< static_cast<uint32_t>(idata->opcode);
	throw ss.str();
}

};
