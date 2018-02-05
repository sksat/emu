#ifndef X86_INSTRUCTION16_H_
#define X86_INSTRUCTION16_H_

#include "instruction.h"

namespace x86 {

class Instruction16 : public x86::Instruction {
public:
	Instruction16(x86::Emulator *e) : x86::Instruction(e) {}
	void Init();

	void add_rm16_imm8(){
		DOUT("\n\t"<<__func__<<" ");
		uint16_t rm16 = idata->GetRM16();
		uint16_t result = rm16 + idata->imm8;
		idata->SetRM16(result);
		EFLAGS.UpdateAdd(rm16, idata->imm8, result);
	}
	void cmp_rm16_imm8(){
		uint16_t rm16 = idata->GetRM16();
		EFLAGS.Cmp(rm16, idata->imm8);
	}
	void code_83(){
		DOUT(" REG="<<static_cast<uint32_t>(idata->modrm.reg)<<" ");
		switch(idata->modrm.reg){
		case 0:	add_rm16_imm8();	break;
		case 7: cmp_rm16_imm8();	break;
		default:
			{
				std::stringstream ss;
				ss << "not implemented: 0x83 /"<<static_cast<uint32_t>(idata->modrm.reg);
				throw ss.str();
			}
		}
	}

	void mov_r16_imm16(){
		uint8_t num = idata->opcode & ((1<<3)-1);
		auto& reg = emu->reg[num];
		reg = idata->imm16;
		DOUT(std::endl<<"\t"<<reg.GetName()<<"=0x"<<idata->imm16);
	}
};

};

#endif
