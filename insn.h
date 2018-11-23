#ifndef INSN_H_
#define INSN_H_

#include <functional>
#include <array>
#include "cpu.h"

namespace insn {
	enum iflag_t {
		none = 0b00000000,
		imm8 = 0b00000001, rel8=imm8,
		imm  = 0b00000010,
		imm16= 0b00000100,
		imm32= 0b00001000,
	};

	using ifunc_t = std::function<void(CPU&,std::vector<uint8_t>&)>;
	inline std::array<const char*, 256>	name;
	inline std::array<iflag_t, 256>		flag;
	inline std::array<ifunc_t,256>		func;

	void init();
}

#endif
