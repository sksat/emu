#ifndef CPU_H_
#define CPU_H_

#include <cstdint>
#include <vector>

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

struct InsnData {
	struct ModRM {
		uint8_t rm  : 3;
		uint8_t reg : 3;
		uint8_t mod : 2;
	};
	struct SIB {
		uint8_t base  : 3;
		uint8_t index : 3;
		uint8_t scale : 2;
	};

	uint8_t prefix;
	uint8_t opcode;

	union {
		uint8_t _modrm;
		ModRM modrm;
	};

	union {
		uint8_t _sib;
		SIB sib;
	};

	union {
		uint8_t  disp8;
		uint16_t disp16;
		uint32_t disp32;
	};

	union {
		uint8_t  imm8;
		uint16_t imm16;
		uint32_t imm32;
	};
};

class CPU {
public:
	void fetch(std::vector<uint8_t> &memory);
	void decode();
//	void exec(std::vector<uint8_t> &memory);

	Register reg_pc; // program counter
	Register reg[8];

	InsnData idata;
};

#ifndef REG
	#define REG cpu.reg
#endif

// join macro
#define JOIN(a, b) JOIN_INTERNAL(a, b)
#define JOIN_INTERNAL(a, b) a ## b

// 32bit register access
#define EIP JOIN(REG, _pc.r32)

//#define EIP REG ## _pc.r32
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

#endif
