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
		static const uint8_t None	= 0b000000;
		static const uint8_t ModRM	= 0b000001;
		static const uint8_t Imm8	= 0b000010;
		static const uint8_t Imm16	= 0b000100;
		static const uint8_t Imm32	= 0b001000;
		static const uint8_t Moffs	= 0b010000;
		static const uint8_t Ptr16	= 0b100000;
	};
	uint8_t insn_flgs[256];

	void not_impl_insn();

	void code_0f(){
		DOUT(std::endl<<"subop=0x"<<std::hex<<static_cast<uint32_t>(idata->subopcode));
		switch(idata->subopcode){
		case 0x01: code_0f01(); break;
		case 0x20: mov_r32_crn(); break;
		case 0x22: mov_crn_r32(); break;
		default:
			throw "not implemented: 0x0f subop";
		}
	}
		void code_0f01(){
			DOUT(" /"<<static_cast<uint32_t>(idata->modrm.reg));
			switch(idata->modrm.reg){
			case 2: lgdt(); break;
			default:
				std::stringstream ss;
				ss << "not implemented: 0x0f01 /"
					<< static_cast<uint32_t>(idata->modrm.reg);
				throw ss.str();
				break;
			}
		}
			void lgdt(){
				uint32_t addr = idata->CalcMemAddr();
				// GDTR.limit = SRC[0:15];
				uint16_t limit;
				uint32_t base;
				if(!idata->is_op32){
					uint16_t addr16 = static_cast<uint16_t>(addr);
					addr = addr16;
					limit = emu->memory->GetData16(addr16);
					// GDTR.base  = SRC[16:47] & 0xffffff;
					base  = emu->memory->GetData32(addr16+2) & 0xffffff;
				}else{
					limit = emu->memory->GetData16(addr);
					// GDTR.base  = SRC[16:47];
					base  = emu->memory->GetData32(addr+2);
				}
				emu->GDTR.limit = limit;
				emu->GDTR.base  = base;

				DOUT(std::endl<<"LGDT: GDTR <- [0x"<<std::hex<<addr<<"](limit=0x"<<std::hex<<limit<<", base=0x"<<std::hex<<base<<")");
			}
		void mov_r32_crn(){
			auto& r32 = emu->reg[idata->RM];
			auto n   = idata->modrm.reg;
			uint32_t crn = GET_CRN(n);
			r32.reg32 = crn;
			DOUT(std::endl<<__func__<<": "<<r32.GetName()<<" <- CR"<<static_cast<uint32_t>(n)<<"(0x"<<std::hex<<crn<<")");
		}
		void mov_crn_r32(){
			auto& r32 = emu->reg[idata->RM];
			auto n = idata->modrm.reg;
			SET_CRN(n, r32.reg32);
			DOUT(std::endl<<__func__<<": CR"<<static_cast<uint32_t>(n)<<" <- "<<r32.GetName()<<"(0x"<<std::hex<<r32.reg32<<")");
			if(emu->CR0.PE)
				DOUT(std::endl<<"Protect Enable");
			if(emu->CR0.PG)
				throw "not implemented: paging";
		}

	void and_al_imm8(){
		uint8_t al = AL;
		AL = al & idata->imm8;
		EFLAGS.UpdateAnd(al, idata->imm8, al&idata->imm8);
	}

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

	void mov_rm8_r8(){
		uint8_t reg8 = GET_REG8(idata->modrm.reg);
		DOUT(__func__<<": "<<GET_REG8_NAME(idata->modrm.reg)<<"=0x"<<std::hex<<reg8);
		idata->SetRM8(reg8);
	}

	void mov_r8_rm8(){
		auto rm8 = idata->GetRM8();
		SET_REG8(idata->modrm.reg, rm8);
		DOUT(std::endl<<"\t"<<GET_REG8_NAME(idata->modrm.reg)<<"=0x"<<std::hex<<static_cast<uint32_t>(rm8));
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

	void code_c0(){
		switch(idata->modrm.reg){
		case 5: shr_rm8_imm8(); break;
		default:
			std::stringstream ss;
			ss << "not implemented: 0xc0 /"
				<< std::dec << static_cast<uint32_t>(idata->modrm.reg);
			throw ss.str();
		}
	}
		void shr_rm8_imm8(){
			uint8_t rm8 = idata->GetRM8();
			uint64_t result = rm8 >> idata->imm8;
			idata->SetRM8(result);
			EFLAGS.UpdateShr(rm8, idata->imm8, result);
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

	void out_dx_al(){
		emu->io->out8(DX, AL);
	}

	void hlt(){
		std::cout<<"hlt"<<std::endl;
		emu->halt_flg = true;
	}

	void cli(){
		EFLAGS.IF = 0;
	}
};

};

#endif
