#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include <stdint.h>

class Emulator;

typedef void instruction_func_t(Emulator*);

class Instruction {
public:
	Instruction(){ Init(); }
	~Instruction(){}
	virtual void Init(void);
	void InitCommonInsn(void);
	void Exec(Emulator *emu, uint8_t code);
protected:
	instruction_func_t* insn_func[256];
};

#endif
