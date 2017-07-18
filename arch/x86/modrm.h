#ifndef MODRM_H_
#define MODRM_H_

#include <stdint.h>
#include "emulator.h"

namespace x86 {

class ModRM {
public:
	ModRM(x86::Emulator *e);
	ModRM(x86::Emulator *e, uint8_t modrm);
	inline uint8_t GetRM();
	inline void Parse();
private:
	x86::Emulator *emu;
	uint8_t rm,reg,mod;
	uint8_t _modrm;
public:
	
};

};

#endif
