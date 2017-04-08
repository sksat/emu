#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include <stdint.h>

class Emulator;

class Instruction {
public:
	virtual void Init(void);
	void Exec(Emulator *emu, uint8_t code);
private:
	typedef void (*instruction_func_t)(Emulator *emu);
	instruction_func_t insn_func[0xff];
};

#endif
