#ifndef CPU_H_
#define CPU_H_

#include <cstdint>
#include <vector>
#include <memory>
#include "memory.h"

struct Register {
	explicit Register() : r32(0x00) {}
	union {
		uint32_t r32;
		struct {
			union {
				uint16_t r16;
				struct {
					uint8_t r8;
					uint8_t h8;
				};
			};
			uint16_t h16;
		};
	};
};

struct SRegister {
	explicit SRegister() : r16(0x00) {}
	union {
		uint16_t r16;
		struct {
			uint8_t  RPL	: 2;
			uint8_t  TI		: 1;
			uint16_t index	: 13;
		};
	};
};

struct Eflags {
	explicit Eflags() : r32(0x00) {}
	union {
		uint32_t r32;
		struct {
			bool CF : 1;
			bool _r1: 1;
			bool PF : 1;
			bool _r2: 1;
			bool AF : 1;
			bool _r3: 1;
			bool ZF : 1;
			bool SF : 1;
			bool TF : 1;
			bool IF : 1;
			bool DF : 1;
			bool OF : 1;
			uint8_t IOPL : 2;
			bool NT : 1;
			bool _r4: 1;
			bool RF : 1;
			bool VM : 1;
			bool AC : 1;
			bool VIF: 1;
			bool VIP: 1;
			bool ID : 1;
		};
	};
};

struct InsnData {
	struct ModRM {
		bool is_reg;
		Memory::addr_t addr;
		union {
			uint8_t raw;
			struct {
				uint8_t rm  : 3;
				uint8_t reg : 3;
				uint8_t mod : 2;
			};
		};
	};
	struct SIB {
		union {
			uint8_t raw;
			struct {
				uint8_t base  : 3;
				uint8_t index : 3;
				uint8_t scale : 2;
			};
		};
	};

	uint32_t size = 0;

	uint8_t prefix, prefix2;
	uint8_t opcode;

	ModRM modrm;
	SIB sib;

	union {
		uint8_t  disp8;
		uint16_t disp16;
		uint32_t disp32 =0x00;
	};

	union {
		uint8_t  imm8;
		uint16_t imm16;
		uint32_t imm32 = 0x00;
	};
};

struct CPU {
	Register	reg_pc; // program counter
	Register	reg[8];
	Eflags		eflags;
	SRegister	sreg[6]; // segment register

	InsnData idata;

	std::shared_ptr<Memory> memory;

	bool halt_flag = false;

	void fetch_prefix(const int n);
	void fetch_decode();
	void exec();

	void parse_modrm();
	void parse_modrm32();

	void dump_registers() const;
};

#ifndef REG
	#define REG cpu.reg
#endif
#ifndef SREG
	#define SREG cpu.sreg
#endif

// join macro
#define JOIN(a, b) JOIN_INTERNAL(a, b)
#define JOIN_INTERNAL(a, b) a ## b

// 32bit register access
#define EIP JOIN(REG, _pc.r32)

#define EAX	REG[0].r32
#define ECX REG[1].r32
#define EDX REG[2].r32
#define EBX REG[3].r32
#define ESP REG[4].r32
#define EBP REG[5].r32
#define ESI REG[6].r32
#define EDI REG[7].r32

// 16bit register access
#define IP J(REG, _pc.r16)
#define AX REG[0].r16
#define CX REG[1].r16
#define DX REG[2].r16
#define BX REG[3].r16
#define SP REG[4].r16
#define BP REG[5].r16
#define SI REG[6].r16
#define DI REG[7].r16

// 8bit register access
#define AL REG[0].r8
#define CL REG[1].r8
#define DL REG[2].r8
#define BL REG[3].r8
#define AH REG[0].h8
#define CH REG[1].r8
#define DH REG[2].r8
#define BH REG[3].r8

// segment register access
#define ES SREG[0]
#define CS SREG[1]
#define SS SREG[2]
#define DS SREG[3]
#define FS SREG[4]
#define GS SREG[5]

// insn data access
#define IDATA	cpu.idata
#define _MODRM	IDATA._modrm
#define MODRM	IDATA.modrm
#define _SIB	IDATA._sib
#define SIB		IDATA.sib
#define DISP8	IDATA.disp8
#define DISP16	IDATA.disp16
#define DISP32	IDATA.disp32
#define IMM8	IDATA.imm8
#define IMM16	IDATA.imm16
#define IMM32	IDATA.imm32

#endif
