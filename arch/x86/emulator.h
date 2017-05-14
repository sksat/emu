#ifndef X86_EMULATOR_H_
#define X86_EMULATOR_H_

#include "../../emulator_base.h"

namespace x86 {

class Emulator : public EmulatorBase {
public:
	void Init();
	void InitInstructions();
};

};

#endif
