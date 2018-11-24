#ifndef INSN_H_
#define INSN_H_

#include <functional>
#include <array>
#include "cpu.h"
#include "memory.h"

namespace insn {
	enum iflag_t {
		None = 0b00000000,
		ModRM= 0b00000001,
		Imm8 = 0b00000010, Rel8=Imm8,
		Imm  = 0b00000100,
		Imm16= 0b00001000,
		Imm32= 0b00010000,
	};

	using ifunc_t = std::function<void(CPU&, std::shared_ptr<Memory>)>;
	inline std::array<const char*, 256>	name;
	inline std::array<iflag_t, 256>		flag;
	inline std::array<ifunc_t,256>		func;

	void init();
}

#endif
