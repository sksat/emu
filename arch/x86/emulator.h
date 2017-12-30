#ifndef X86_EMULATOR_H_
#define X86_EMULATOR_H_

#include "../../emulator_base.h"
#include "register.h"

#define EIP pc
#define ESP reg[4]
#define EBP reg[5]

namespace x86 {

const size_t REG_COUNT = 8;
const size_t SREG_COUNT= 6;

class Emulator : public EmulatorBase {
public:
	void InitInstructions();
	void InitRegisters();
	void InitMemory();

	x86::Register32 pc;
	x86::EFLAGS	eflags;
	std::vector<x86::Register32> reg;
	std::vector<x86::Register16> sreg;

	inline uint8_t GetCode8(int index)		{ return (*memory)[EIP + index]; }
	inline int8_t GetSignCode8(int index)	{ return static_cast<int8_t>(GetCode8(index)); }
	inline uint32_t GetCode32(int index)	{
		uint32_t ret = 0;
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
