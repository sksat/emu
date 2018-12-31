#include "insn.h"
#include "util.h"

// ModR/M
#define GET_RM8()	cpu.get_rm8()
#define GET_RM16()	cpu.get_rm16()
#define GET_RM32()	cpu.get_rm32()
#define SET_RM8(v)	cpu.set_rm8(v)
#define SET_RM16(v)	cpu.set_rm16(v)
#define SET_RM32(v)	cpu.set_rm32(v)
//reg
#define REG_NUM	(cpu.idata.modrm.reg)
#define R16	(cpu.reg[REG_NUM].r16)
#define R32	(cpu.reg[REG_NUM].r32)

// EFLAGS
#define ADD(v1,v2)	cpu.eflags.update_add(v1,v2)
#define SUB(v1,v2)	cpu.eflags.update_sub(v1,v2)
#define AND(v1,v2)	cpu.eflags.update_and(v1,v2)
#define OR(v1,v2)	cpu.eflags.update_or(v1,v2)
#define CMP(v1,v2)	cpu.eflags.update_cmp(v1,v2)

void insn::init(){

#define NOT_IMPL { \
	throw std::runtime_error("not implemented insn: " \
				+hex2str(cpu.idata.opcode,1)); \
	}

	for(size_t op=0x00;op<=0xff;op++){
		name[op] = "unknown";
		flag[op] = None;
		func[op] = [](CPU &cpu, std::shared_ptr<Memory> memory) NOT_IMPL;
	}

#define INSN(op, i, f, block) \
	name[op] = #i; \
	flag[op] = f; \
	func[op] = [](CPU &cpu, std::shared_ptr<Memory> memory) block;

	INSN(0x00, add_rm8_r8,	ModRM,{ SET_RM8( ADD(GET_RM8(), GET_R8(REG_NUM)) ); });
	INSN(0x04, add_al_imm8,	Imm8, { AL = ADD(AL, IMM8); });
	INSN(0x0c, or_al_imm8,	Imm8, { AL = OR( AL, IMM8); });
	INSN(0x24, and_al_imm8,	Imm8, { AL = AND(AL, IMM8); });
	INSN(0x2c, sub_al_imm8,	Imm8, { AL = SUB(AL, IMM8); });
	INSN(0x3c, cmp_al_imm8, Imm8, { CMP(AL, IMM8); });
	INSN(0x90, nop,			None, {});
	INSN(0xeb, jmp_rel8,	Rel8, { EIP= EIP + static_cast<int32_t>(IMM32); });
	INSN(0xf4, hlt, None, { cpu.halt_flag=true; });
}
