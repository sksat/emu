#ifndef X86_EMULATOR_H_
#define X86_EMULATOR_H_

#include "../../emulator_base.h"
#include "register.h"

// 32bit registers
#define EFLAGS emu->eflags
#define EIP emu->pc.reg32
#define EAX emu->reg[0].reg32
#define ECX emu->reg[1].reg32
#define EDX emu->reg[2].reg32
#define EBX emu->reg[3].reg32
#define ESP emu->reg[4].reg32
#define EBP emu->reg[5].reg32
#define ESI emu->reg[6].reg32
#define EDI emu->reg[7].reg32

// 16bit registers
#define IP emu->pc.reg16
#define AX emu->reg[0].reg16
#define CX emu->reg[1].reg16
#define DX emu->reg[2].reg16
#define BX emu->reg[3].reg16
#define SP emu->reg[4].reg16
#define BP emu->reg[5].reg16
#define SI emu->reg[6].reg16
#define DI emu->reg[7].reg16

// 8bit registers
#define AL emu->reg[0].low8
#define CL emu->reg[1].low8
#define DL emu->reg[2].low8
#define BL emu->reg[3].low8
#define AH emu->reg[0].high8
#define CH emu->reg[1].high8
#define DH emu->reg[2].high8
#define BH emu->reg[3].high8

#define SET_REG8(num, val) (num<0x4 ? (emu->reg[num].low8=val) : (emu->reg[num-0x4].high8=val))

// segment registers
#define ES emu->sreg[0].reg16
#define CS emu->sreg[1].reg16
#define SS emu->sreg[2].reg16
#define DS emu->sreg[3].reg16
#define FS emu->sreg[4].reg16
#define GS emu->sreg[5].reg16

namespace x86 {

const size_t REG_COUNT = 8;
const size_t SREG_COUNT= 6;

class Emulator : public EmulatorBase {
public:
	void InitInstructions();
	void InitRegisters();
	void InitMemory();
protected:
	size_t mode;
	Emulator *emu = this;
public:
	x86::Register32 pc;
	x86::Eflags	eflags;
	std::vector<x86::Register32> reg;
	std::vector<x86::SRegister> sreg;

	inline bool IsMode16(){ return (mode == 16); }
	inline bool IsMode32(){ return (mode == 32); }

	// memoryの関数を使うべき
	inline uint8_t GetCode8(int index)	{ return (*memory)[EIP + index]; }
	inline int8_t GetSignCode8(int index)	{ return static_cast<int8_t>(GetCode8(index)); }
	inline uint16_t GetCode16(int index){
		uint16_t ret = 0x00;
		for(int i=0;i<2;i++)
			ret |= GetCode8(index + i) << (i * 8);
		return ret;
	}
	inline int16_t GetSignCode16(int index)	{ return static_cast<int16_t>(GetCode16(index)); }
	inline uint32_t GetCode32(int index){
		uint32_t ret = 0x00;
		for(int i=0;i<4;i++)
			ret |= GetCode8(index + i) << (i * 8);
		return ret;
	}
	inline int32_t GetSignCode32(int index)	{ return static_cast<int32_t>(GetCode32(index)); }

	inline void push32(uint32_t val){
		ESP -= 4;
		memory->SetData32(ESP, val);
	}
	inline uint32_t pop32(){
		ESP += 4;
		return memory->GetData32(ESP-4);
	}
};

};

#endif
