#include "insn.h"
#include "util.h"

void insn::init(){

#define NOT_IMPL { \
	throw std::runtime_error("not implemented insn: " \
				+hex2str(cpu.idata.opcode,1)); \
	}

	for(size_t op=0x00;op<=0xff;op++){
		name[op] = "unknown";
		flag[op] = None;
		func[op] = [](CPU &cpu, Memory &memory) NOT_IMPL;
	}

#define INSN(op, i, f, block) \
	name[op] = #i; \
	flag[op] = f; \
	func[op] = [](CPU &cpu, Memory &memory) block;

	INSN(0x00, add_rm8_r8,	ModRM,{ throw std::runtime_error("hoge");});
	INSN(0x04, add_al_imm8,	Imm8, { AL = AL + IMM8; });
	INSN(0x0c, or_al_imm8,	Imm8, { AL = AL | IMM8; });
	INSN(0x24, and_al_imm8,	Imm8, { AL = AL & IMM8; });
	INSN(0x2c, sub_al_imm8,	Imm8, { AL = AL - IMM8; });
	INSN(0x90, nop,			None, {});
	INSN(0xeb, jmp_rel8,	Rel8, { EIP= EIP + static_cast<int32_t>(IMM32); });
	INSN(0xf4, hlt, None, { cpu.halt_flag=true; });
}
