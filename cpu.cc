#include <stdexcept>
#define REG this->reg
#include "cpu.h"
#include "util.h"

void CPU::fetch(std::vector<uint8_t> &memory){
	uint8_t opcode = memory[EIP];

	switch(opcode){
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
			idata.prefix = opcode;
			idata.opcode = memory[EIP+1];
			throw std::runtime_error("unknown prefix: "+hex2str(opcode, 1));
		case 0x0f:
			throw std::runtime_error("two byte insn");
		default:
			idata.opcode = opcode;
			break;
	}
}

void CPU::decode(){
	const auto &op = idata.opcode;
	switch(op){
		default:
			throw std::runtime_error("unknown opcode: "+hex2str(op, 1));
	}
}
