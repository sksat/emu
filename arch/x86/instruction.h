#ifndef X86_INSTRUCTION_H_
#define X86_INSTRUCTION_H_

#include <stdio.h>
#include "../../insn_base.h"
#include "emulator.h"
#include "insndata.h"

namespace x86 {

class Instruction : public InstructionBase {
public:
	Instruction(x86::Emulator *e, x86::InsnData *i) : emu(e), idata(i) { insn_name.resize(256); }
	~Instruction(){}
	virtual void Init();
	void Fetch();
	void Decode();
	void Exec();
protected:
	x86::Emulator *emu;
	x86::InsnData *idata;
	struct Flag {
		static const uint8_t None	= 0b00000;
		static const uint8_t ModRM	= 0b00001;
		static const uint8_t Imm8	= 0b00010;
		static const uint8_t Imm16	= 0b00100;
		static const uint8_t Imm32	= 0b01000;
		static const uint8_t Moffs	= 0b10000;
	};
	uint8_t insn_flgs[256];

	void not_impl_insn();

	void cmp_al_imm8(){
		uint8_t al = AL;
		EFLAGS.Cmp(al, idata->imm8);
	}

// Jump if Condition Is Met
#define DEF_JCC_REL8(flag, is_flag) \
void j ## flag ## _rel8(){ \
	if(is_flag) EIP += static_cast<int8_t>(idata->imm8); \
}

	DEF_JCC_REL8(o,  EFLAGS.OF);
	DEF_JCC_REL8(no, !EFLAGS.OF);
	DEF_JCC_REL8(b,  EFLAGS.CF);					// jb =jc =jnae
	DEF_JCC_REL8(ae, !EFLAGS.CF);					// jae=jnb=jnc
	DEF_JCC_REL8(e,  EFLAGS.ZF);					// je =jz
	DEF_JCC_REL8(ne, !EFLAGS.ZF);					// jne=jnz
	DEF_JCC_REL8(be, EFLAGS.CF || EFLAGS.ZF);			// jbe=jna
	DEF_JCC_REL8(a,  !EFLAGS.CF && !EFLAGS.ZF);			// ja =jnbe
	DEF_JCC_REL8(s,  EFLAGS.SF);
	DEF_JCC_REL8(ns, !EFLAGS.SF);
	DEF_JCC_REL8(p,  EFLAGS.PF);					// jp =jpe
	DEF_JCC_REL8(po, !EFLAGS.PF);
	DEF_JCC_REL8(l,  EFLAGS.SF != EFLAGS.OF);			// jl =jnge
	DEF_JCC_REL8(ge, EFLAGS.SF == EFLAGS.OF);			// jge=jnl
	DEF_JCC_REL8(le, EFLAGS.ZF || (EFLAGS.SF != EFLAGS.OF));	// jle=jng
	DEF_JCC_REL8(g,  !EFLAGS.ZF && (EFLAGS.SF == EFLAGS.OF));	// jg = jnle

	void add_rm8_imm8(){
		auto rm8 = idata->GetRM8();
		uint8_t tmp = rm8 + idata->imm8;
		idata->SetRM8(tmp);
		EFLAGS.UpdateAdd(rm8, idata->imm8, tmp);
	}
	void cmp_rm8_imm8(){
		EFLAGS.Cmp(idata->GetRM8(), idata->imm8);
	}
	void code_80(){
		DOUT(" REG="<<static_cast<uint32_t>(idata->modrm.reg)<<" ");
		switch(idata->modrm.reg){
		case 0:	add_rm8_imm8();	break;
		case 7: cmp_rm8_imm8();	break;
		default:
		{
			std::stringstream ss;
			ss << "not implemented: 0x80 /"<<static_cast<uint32_t>(idata->modrm.reg);
			throw ss.str();
		}
		}
	}

	void mov_r8_rm8(){
		auto rm8 = idata->GetRM8();
		auto& r8 = emu->reg[idata->modrm.reg];
		r8 = rm8;
		DOUT(std::endl<<"\t"<<r8.GetName()<<"=0x"<<std::hex<<static_cast<uint32_t>(rm8));
	}

	void mov_sreg_rm16(){
		auto rm16 = idata->GetRM16();
		auto& sreg = emu->sreg[idata->modrm.reg];
		sreg = rm16;
		DOUT(std::endl<<"\t"<<sreg.GetName()<<"=0x"<<std::hex<<rm16);
	}

	void nop(){}

	void mov_moffs8_al(){
		emu->memory->SetData8(idata->moffs, AL);
		DOUT("[0x"<<std::hex<<idata->moffs<<"] = AL("<<static_cast<uint32_t>(AL)<<")"<<std::endl);
	}

/* TODO: this is 32bit op
	void near_jump(){
		int32_t diff = emu->GetCode32(1);
		EIP += (diff + 4);
	}
*/
	void mov_r8_imm8(){
		uint8_t reg8 = idata->opcode - 0xb0;
		SET_REG8(reg8, idata->imm8);
	}

	void mov_rm8_imm8(){
		idata->SetRM8(idata->imm8);
	}

	void int_imm8(){
		DOUT(std::endl<<"\tint "<<static_cast<uint32_t>(idata->imm8));
		emu->bios->Function(static_cast<int>(idata->imm8));
	}

	void in_al_imm8(){
		uint8_t port = idata->imm8;
		AL = emu->io->in8(port);
	}

	void out_imm8_al(){
		uint8_t port = idata->imm8;
		emu->io->out8(port, AL);
	}

	void short_jump(){
		IP += static_cast<uint16_t>(idata->imm8);
	}

	void hlt(){
		DOUT("hlt"<<std::endl);
		emu->halt_flg = true;
	}

	void cli(){
		EFLAGS.IF = 0;
	}
};

};

#endif
