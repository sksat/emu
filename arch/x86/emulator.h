#ifndef X86_EMULATOR_H_
#define X86_EMULATOR_H_

#include "../../emulator_base.h"
#include "register.h"

#define EIP emu->pc
#define EAX emu->reg[0]
#define ECX emu->reg[1]
#define EDX emu->reg[2]
#define EBX emu->reg[3]
#define ESP emu->reg[4]
#define EBP emu->reg[5]
#define ESI emu->reg[6]
#define EDI emu->reg[7]

#define IP static_cast<Register16&>(emu->pc)
#define AX static_cast<Register16&>(emu->reg[0])
#define CX static_cast<Register16&>(emu->reg[1])
#define DX static_cast<Register16&>(emu->reg[2])
#define BX static_cast<Register16&>(emu->reg[3])
#define SP static_cast<Register16&>(emu->reg[4])
#define BP static_cast<Register16&>(emu->reg[5])
#define SI static_cast<Register16&>(emu->reg[6])
#define DI static_cast<Register16&>(emu->reg[7])

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
	x86::EFLAGS	eflags;
	std::vector<x86::Register32> reg;
	std::vector<x86::Register16> sreg;

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
