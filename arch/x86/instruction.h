#ifndef X86_INSTRUCTION_H_
#define X86_INSTRUCTION_H_

#include <stdio.h>
#include "../../insn_base.h"
#include "emulator.h"
#include "insndata.h"

namespace x86 {

class Instruction : public InstructionBase {
public:
	Instruction(x86::Emulator *e);
	~Instruction(){ delete idata; }
	virtual void Init();
	void Parse();
	void ExecStep();
protected:
	x86::Emulator *emu;
	x86::InsnData *idata;
	struct Flag {
		static const uint8_t None	= 0b0000;
		static const uint8_t ModRM	= 0b0001;
		static const uint8_t Imm8	= 0b0010;
		static const uint8_t Imm16	= 0b0100;
		static const uint8_t Imm32	= 0b1000;
	};
	uint8_t insn_flgs[256];

	void not_impl_insn();

	void cmp_al_imm8(){
		uint8_t al = AL;
		EFLAGS.Cmp(al, idata->imm8);
	}

#define DEFINE_JX(flag, is_flag) \
void j ## flag(){ \
	if(emu->eflags.is_flag()) EIP += static_cast<int8_t>(idata->imm8); \
} \
void jn ## flag(){ \
	if(!emu->eflags.is_flag()) EIP += static_cast<int8_t>(idata->imm8); \
}

	DEFINE_JX(o, IsOverflow);
	DEFINE_JX(c, IsCarry);
	DEFINE_JX(z, IsZero);
	DEFINE_JX(s, IsSign);

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

	void int_imm8(){
		DOUT(std::endl<<"\tint "<<static_cast<uint32_t>(idata->imm8));
		emu->bios->Function(static_cast<int>(idata->imm8));
	}

	void short_jump(){
		IP += static_cast<uint16_t>(idata->imm8);
	}

	void hlt(){
		DOUT("hlt"<<std::endl);
	//	for(;;){}
		emu->finish_flg = true;
	}
};

};

#endif
