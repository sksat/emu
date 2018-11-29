#include <iostream>
#include <stdexcept>
#define REG this->reg
#define SREG this->sreg
#include "cpu.h"
#include "insn.h"
#include "util.h"

void CPU::fetch_prefix(int n=0){
	auto& size = idata.size;
	uint8_t prefix = memory->get8(EIP+size);
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
			fetch_prefix(n+1);
			break;
unknown:
			throw std::runtime_error("unknown prefix: "
					+(n==0 ? hex2str(prefix,1) : (hex2str(idata.prefix,1)+","+hex2str(prefix,1))));
		default:
			return;
	}
}

void CPU::fetch_decode(){
	idata = {};
	if(EIP >= memory->get_size())
		throw std::runtime_error("memory over");
	fetch_prefix();
	auto& size = idata.size;

	idata.opcode = memory->get8(EIP+size);
	size++;

	std::cout << "[" << hex2str(idata.opcode,1) << "] "
		<< insn::name[idata.opcode] << std::endl;

	const auto &op = idata.opcode;
	const auto &flag = insn::flag[op];

	if(flag & insn::ModRM){
		idata.modrm.raw = memory->get8(EIP+size);
		size++;
		parse_modrm();
	}

	if(flag & insn::Imm8){
		idata.imm8 = memory->get8(EIP+size);
		size++;
		std::cout << "\timm8: " + hex2str(idata.imm8, 1) << std::endl;
	}

	EIP = EIP + size;
}

void CPU::exec(){
	insn::func[idata.opcode](*this, memory);
}

void CPU::parse_modrm(){
	auto& modrm = idata.modrm;
	std::cout << "\tModRM[" << hex2str(modrm.raw,1) << "]"
		<< "(Mod=" << hex2str(modrm.mod,1)
		<< ", Reg/Op=" << hex2str(modrm.reg,1)
		<< ", RM=" << hex2str(modrm.rm,1)
		<< ")" << std::endl;
	parse_modrm32();
	std::cout << "\t -> ";
	if(modrm.is_reg)
		std::cout << "reg[" << static_cast<int>(modrm.rm) << "]";
	else
		std::cout << "mem[" << hex2str(modrm.addr) << "]";
	std::cout << std::endl;
}

void CPU::parse_modrm32(){
	auto& size	= idata.size;
	auto& modrm	= idata.modrm;
	auto& sib	= idata.sib;
#define MOD	modrm.mod
#define RM	modrm.rm

	if(MOD != 0b11 && RM == 0b100){
		throw std::runtime_error("sib");
	}

	// (MOD, RM)
	// (00, 100) -> [sib]
	// (00, 101) -> [disp32]
	// (00,   *) -> [reg(modrm.reg)]
	// (01, 100) -> [sib+disp8]
	// (01, 101) -> [SS:EBP+disp8]
	// (01,   *) -> [reg(modrm.reg)+disp8]
	// (10, 100) -> [sib+disp32]
	// (10, 101) -> [SS:EBP+disp8]
	// (11,   *) -> reg(rm)

	if(MOD==0b11){ // reg(rm)
		modrm.is_reg = true;
		return;
	}
	modrm.is_reg = false;
	modrm.addr = 0x00000000;

	if(RM==0b100){ // SIB
		idata.sib.raw = memory->get8(EIP+size);
		size++;
		std::cout << "\tSIB[" << hex2str(sib.raw,1) << "]"
			<< "(scale=" << hex2str(sib.scale,1)
			<< ", index=" << hex2str(sib.index,1)
			<< ", base=" << hex2str(sib.base,1)
			<< ")" << std::endl;
		// TODO: calc SIB addr
		modrm.addr = calc_sib_addr();
		if(MOD==0b00) return;
		if(MOD==0b01){
			idata.disp32 = 0x00;
			idata.disp8 = memory->get8(EIP+size);
			size++;
		}else if(MOD==0b10){
			idata.disp32= memory->get32(EIP+size);
			size+=4;
		}
		modrm.addr += idata.disp32;
		return;
	}else if(RM==0b101){
		if(MOD==0b00){
			idata.disp32 = memory->get32(EIP+size);
			size+=4;
			modrm.addr = idata.disp32;
			return;
		}
		idata.disp8 = memory->get8(EIP+size);
		size++;
		modrm.addr = EBP + idata.disp8;
		//TODO: sreg=SS
		return;
	}

	if(MOD==0b01){
		idata.disp8 = memory->get8(EIP+size);
		size++;
		modrm.addr = idata.disp8;
	}

	modrm.addr += reg[modrm.reg].r32;
}

const Memory::addr_t CPU::calc_sib_addr() const {
	Memory::addr_t addr = 0x00;
	const auto& modrm = idata.modrm;
	const auto& sib = idata.sib;

	if(sib.base == 0b101){
		if(modrm.mod != 0b00)
			addr += EBP;
	}else{
		addr += REG[sib.base].r32;
	}

	if(sib.index == 0b100)
		return addr;
	else if(sib.index == 0b101){
		//TODO: sreg=SS
	}

	const auto& index32 = REG[sib.index].r32;
	addr += index32 * (1 << sib.scale);
	return addr;
}

void CPU::dump_registers() const {
	using std::endl;
	std::cout
		<< "----- dump registers-----" << endl
#define DUMP(reg) #reg":\t" << hex2str(reg) << endl
		<< DUMP(EIP)
		<< "EFLAGS:\t" << hex2str(eflags.r32) << endl
		<< "\t|CF|PF|AF|ZF|SF|TF|IF|DF|OF|IOPL|NT|RF|VM|AC|VIF|VIP|ID|" << endl
#define F(f) (eflags.f ? "| 1" : "| 0")
		<< "\t" <<F(CF)<<F(PF)<<F(AF)<<F(ZF)<<F(SF)<<F(TF)<<F(IF)<<F(DF)<<F(OF)
		<< "|  " <<((eflags.IOPL>>1)?"1":"0")<<((eflags.IOPL&1)?"1":"0")
		<<F(NT)<<F(RF)<<F(VM)<<F(AC)<<F(VIF)<<" "<<F(VIP)<<" "<<F(ID)<<"|"<<endl
		<< DUMP(EAX)
		<< DUMP(ECX)
		<< DUMP(EDX)
		<< DUMP(EBX)
		<< DUMP(ESP)
		<< DUMP(EBP)
		<< DUMP(ESI)
		<< DUMP(EDI)
#define SDUMP(sreg) #sreg":\t" << hex2str(sreg.r16,2) \
		<< " = (RPL=" + hex2str(sreg.RPL,1) \
		+ ", TI="+hex2str(sreg.TI,1) \
		+ ", index="+hex2str(sreg.index,2) + ")" << endl
		<< SDUMP(ES)
		<< SDUMP(CS)
		<< SDUMP(SS)
		<< SDUMP(DS)
		<< SDUMP(FS)
		<< SDUMP(GS)
		<< "-------------------------" << endl;
}
