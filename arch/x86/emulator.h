#include "../../emulator_base.h"

namespace x86 {

class Emulator : public EmulatorBase {
public:
	void Init();
	void InitInstructions();
};

};
