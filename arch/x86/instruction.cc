#include <sstream>
#include "instruction.h"
#include "emulator.h"

#define SETINSN(op,func,insn_flg) {insn[op] = (insnfunc_t)&x86::Instruction::func; insn_name[op] = #func; insn_flgs[op] = insn_flg;}

namespace x86 {

void Instruction::Init(){
	// default insn
	ClearInsn(256);
	idata->opcode = 0x90;

//	SETINSN(0x00, add_rm8_r8,		Flag::ModRM);
	SETINSN(0x3c, cmp_al_imm8,		Flag::Imm8);
	SETINSN(0x70, jo_rel8,			Flag::Imm8);
	SETINSN(0x71, jno_rel8,			Flag::Imm8);
	SETINSN(0x72, jb_rel8,			Flag::Imm8); // = jc,jnae
	SETINSN(0x73, jae_rel8,			Flag::Imm8); // = jnb,jnc
	SETINSN(0x74, je_rel8,			Flag::Imm8); // = jz
	SETINSN(0x75, jne_rel8,			Flag::Imm8); // = jnz
	SETINSN(0x76, jbe_rel8,			Flag::Imm8); // = jna
	SETINSN(0x77, ja_rel8,			Flag::Imm8); // = jnbe
	SETINSN(0x78, js_rel8,			Flag::Imm8);
	SETINSN(0x79, jns_rel8,			Flag::Imm8);
	SETINSN(0x7a, jp_rel8,			Flag::Imm8); // = jpe
	SETINSN(0x7b, jpo_rel8,			Flag::Imm8);
	SETINSN(0x7c, jl_rel8,			Flag::Imm8); // = jnge
	SETINSN(0x7d, jge_rel8,			Flag::Imm8); // = jnl
	SETINSN(0x7e, jle_rel8,			Flag::Imm8); // = jng
	SETINSN(0x7f, jg_rel8,			Flag::Imm8); // = jnle
	SETINSN(0x80, code_80,			Flag::ModRM | Flag::Imm8);
	SETINSN(0x8a, mov_r8_rm8,		Flag::ModRM);
	SETINSN(0x8e, mov_sreg_rm16,		Flag::ModRM);
	SETINSN(0x90, nop,			Flag::None);
	SETINSN(0xa2, mov_moffs8_al,		Flag::Moffs);
	for(auto i=0;i<8;i++)
		SETINSN(0xb0+i, mov_r8_imm8,	Flag::Imm8);
	SETINSN(0xc6, mov_rm8_imm8,		Flag::ModRM | Flag::Imm8);
	SETINSN(0xcd, int_imm8,			Flag::Imm8);
//	SETINSN(0xe9, near_jump,	0); // TODO: 32bitだったので32bitの方に移す
	SETINSN(0xe6, out_imm8_al,		Flag::Imm8);
	SETINSN(0xeb, short_jump,		Flag::Imm8);
	SETINSN(0xf4, hlt,			Flag::None);
}

void Instruction::Fetch(){
	idata->prefix = idata->opcode = emu->GetCode8(0);

	// parse prefix
	// bochs/cpu/fetchdecode.cc fetchDecode32
	// is_32 = BX_CPU_THIS_PTR sregs[BX_SEG_REG_CS].cache.u.segment.d_b;
	idata->chsiz_op = idata->chsiz_addr = false;
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
			goto not_impl;
		case 0x66:	// op size
			DOUT("0x66: operand size prefix"<<std::endl);
			idata->chsiz_op = true;
			EIP++;
			idata->opcode = emu->GetCode8(0);
			break;
		case 0x67:	// addr size
			DOUT("0x67: address size prefix"<<std::endl);
			idata->chsiz_addr = true;
			EIP++;
			idata->opcode = emu->GetCode8(0);
			break;
not_impl:
		{
			std::stringstream ss;
			ss << "not implemented prefix:"
				<< std::hex << std::showbase
				<< static_cast<uint32_t>(idata->prefix)
				<< std::endl;
			throw ss.str();
		}
		default:
			idata->prefix = 0x00;
			break;
	}
	EIP++;
}

void Instruction::Decode(){
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
				<< static_cast<uint32_t>(idata->MOD)
				<< " REG=0x"
				<< static_cast<uint32_t>(idata->modrm.reg)
				<< " RM=0x"
				<< static_cast<uint32_t>(idata->RM)
				<< "  ");
		EIP++;
		if(emu->IsMode16() ^ idata->chsiz_addr)
			idata->ParseModRM16();
		else
			idata->ParseModRM32();
	}

	// imm
	if(flgs & Flag::Imm8){
		uint8_t data = emu->GetCode8(0);
		idata->imm8 = static_cast<int8_t>(data);
		EIP++;
		DOUT("  imm8=0x"<<std::hex<<static_cast<int32_t>(data));
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
	if(flgs & Flag::Moffs){
		if(emu->IsMode16() ^ idata->chsiz_addr){
			idata->moffs = static_cast<uint32_t>(emu->GetCode16(0));
			EIP+=2;
		}else{
			idata->moffs = emu->GetCode32(0);
			EIP+=4;
		}
	}

}

void Instruction::Exec(){
	insnfunc_t func = insn[idata->opcode];
	(this->*func)();
}

void Instruction::not_impl_insn(){
	std::stringstream ss;
	ss << "x86: not implemented insn : 0x" 
		<< std::setw(2) << std::setfill('0') << std::hex
		<< static_cast<uint32_t>(idata->opcode);
	throw ss.str();
}

};
