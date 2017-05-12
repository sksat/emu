#include <vector>
#include "../../insn_base.h"

namespace x86 {

class Instruction : public InsnBase {
public:
	Instruction(){}
	void Init();
	void Parse();
	void Exec();
};

};
