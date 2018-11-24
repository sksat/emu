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
	size++;

	std::cout << "[" << hex2str(idata.opcode,1) << "] "
		<< insn::name[idata.opcode] << std::endl;

	const auto &op = idata.opcode;
	const auto &flag = insn::flag[op];

	if(flag & insn::ModRM){
		idata._modrm = memory[EIP+size];
		size++;
		std::cout << "\tModRM[" + hex2str(idata._modrm,1) << "] "
			<< "(Mod=" << hex2str(idata.modrm.mod,1)
			<< ", Reg/Op=" << hex2str(idata.modrm.reg,1)
			<< ", RM=" << hex2str(idata.modrm.rm,1)
			<< ")" << std::endl;
	}

	if(flag & insn::Imm8){
		idata.imm8 = memory[EIP+size];
		size++;
		std::cout << "\timm8: " + hex2str(idata.imm8, 1) << std::endl;
	}

	EIP = EIP + size;
}

void CPU::exec(std::vector<uint8_t> &memory){
	insn::func[idata.opcode](*this, memory);
}
