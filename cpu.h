#ifndef CPU_H_
#define CPU_H_

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

class CPU {
public:
	Register pc; // program counter
	Register reg[8];
};

#define REG cpu.reg

// 32bit register access
#define EIP cpu.pc.r32
#define EAX	REG[0].r32
#define ECX REG[1].r32
#define EDX REG[2].r32
#define EBX REG[3].r32
#define ESP REG[4].r32
#define EBP REG[5].r32
#define ESI REG[6].r32
#define EDI REG[7].r32

// 16bit register access
#define IP cpu.pc.r16
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
