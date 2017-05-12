#ifndef OSECPU_INSTRUCTION_H_
#define OSECPU_INSTRUCTION_H_

#include "../../insn_base.h"

namespace osecpu {

class Instruction : public InsnBase {
public:
	Instruction(){}
	void Init();
	void Parse();
	void Exec();
};

};

#endif
