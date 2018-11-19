#include <iostream>
#include <stdexcept>
#define REG this->reg
#include "cpu.h"
#include "insn.h"
#include "util.h"

void CPU::fetch_prefix(const std::vector<uint8_t> &memory, int n=0){
	auto& size = idata.size;
	uint8_t prefix = memory[EIP+size];
	switch(prefix){
		case 0xf0:
		case 0xf2:
		case 0xf3:
		case 0x2e:
		case 0x36:
		case 0x3e:
		case 0x26:
		case 0x64:
		case 0x65:
		case 0x66:
		case 0x67:
			goto unknown;
ok:
			if(n==0) idata.prefix = prefix;
			else idata.prefix2= prefix;
			size++;
			fetch_prefix(memory, n+1);
			break;
unknown:
			throw std::runtime_error("unknown prefix: "
					+(n==0 ? hex2str(prefix,1) : (hex2str(idata.prefix,1)+","+hex2str(prefix,1))));
		default:
			return;
	}
}

void CPU::fetch_decode(const std::vector<uint8_t> &memory){
	idata = {};
	if(EIP >= memory.size())
		throw std::runtime_error("memory over");
	fetch_prefix(memory);
	auto& size = idata.size;

	idata.opcode = memory[EIP+size];
	const auto &op = idata.opcode;

	switch(insn::flag[op]){
		case insn::none:
			size++;
			break;
		default:
			throw std::runtime_error("unknown flag: "+hex2str(insn::flag[op],1));
	}

	EIP = EIP + size;
}

void CPU::exec(std::vector<uint8_t> &memory){
	const auto &op = idata.opcode;

	std::cout << "opcode: " << hex2str(op, 1) << std::endl;

	insn::func[op](*this, memory);
}
