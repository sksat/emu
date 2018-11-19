#include "insn.h"
#include "util.h"

void insn::init(){
	for(size_t op=0x00;op<=0xff;op++){
		name[op] = "";
		flag[op] = none;
		func[op] = [](CPU &cpu, std::vector<uint8_t> &memory){
			throw std::runtime_error("not implemented insn: "
					+hex2str(cpu.idata.opcode,1));
		};
	}

#define INSN(op, i, f, block) \
	name[op] = #i; \
	flag[op] = f; \
	func[op] = [](CPU &cpu, std::vector<uint8_t> &memory) block;

	INSN(0x90, nop, none, {});
	INSN(0xf4, hlt, none, {
			cpu.halt_flag=true;
	});
}
