#ifndef X86_EMULATOR_H_
#define X86_EMULATOR_H_

#include "../../emulator_base.h"
#include "register.h"

// 32bit registers
#define EFLAGS emu->eflags
#define EIP emu->pc.reg32
#define EAX emu->reg[0].reg32
#define ECX emu->reg[1].reg32
#define EDX emu->reg[2].reg32
#define EBX emu->reg[3].reg32
#define ESP emu->reg[4].reg32
#define EBP emu->reg[5].reg32
#define ESI emu->reg[6].reg32
#define EDI emu->reg[7].reg32

// 16bit registers
#define IP emu->pc.reg16
#define AX emu->reg[0].reg16
#define CX emu->reg[1].reg16
#define DX emu->reg[2].reg16
#define BX emu->reg[3].reg16
#define SP emu->reg[4].reg16
#define BP emu->reg[5].reg16
#define SI emu->reg[6].reg16
#define DI emu->reg[7].reg16

// 8bit registers
#define AL emu->reg[0].low8
#define CL emu->reg[1].low8
#define DL emu->reg[2].low8
#define BL emu->reg[3].low8
#define AH emu->reg[0].high8
#define CH emu->reg[1].high8
#define DH emu->reg[2].high8
#define BH emu->reg[3].high8

#define GET_REG8_NAME(num)	(num<0x4 ? (emu->reg[num].GetName()) : (emu->reg[num-0x4].GetName()))
#define GET_REG8(num)		(num<0x4 ? (emu->reg[num].low8) : (emu->reg[num-0x4].high8))
#define SET_REG8(num, val)	(num<0x4 ? (emu->reg[num].low8=val) : (emu->reg[num-0x4].high8=val))

// segment registers
#define ES emu->sreg[0]
#define CS emu->sreg[1]
#define SS emu->sreg[2]
#define DS emu->sreg[3]
#define FS emu->sreg[4]
#define GS emu->sreg[5]

#define GET_CRN(num)		(num==0x00 ? (emu->CR0.reg32) : (throw "GET_CRN: crn not found."))
#define SET_CRN(num, val)	(num==0x00 ? (emu->CR0.reg32=val): (throw "SET_CRN: crn not found."))

// memory access(physical address)
#define GET_MEM8(addr)		(emu->memory->GetData8(addr))
#define GET_MEM16(addr)		(emu->memory->GetData16(addr))
#define GET_MEM32(addr)		(emu->memory->GetData32(addr))

#define SET_MEM8(addr, val)	(emu->memory->SetData8(addr,  static_cast<uint8_t>(val)) )
#define SET_MEM16(addr, val)	(emu->memory->SetData16(addr, static_cast<uint16_t>(val)))
#define SET_MEM32(addr, val)	(emu->memory->SetData32(addr, static_cast<uint32_t>(val)))

// memory access with segmentation
#define GET_SEG_MEM8(sreg, addr)  (GET_MEM8(emu->L2P(sreg, addr)) )
#define GET_SEG_MEM16(sreg, addr) (GET_MEM16(emu->L2P(sreg, addr)))
#define GET_SEG_MEM32(sreg, addr) (GET_MEM32(emu->L2P(sreg, addr)))

#define SET_SEG_MEM8(sreg, addr, val)  (SET_MEM8(emu->L2P(sreg, addr), val))
#define SET_SEG_MEM16(sreg, addr, val) (SET_MEM16(emu->L2P(sreg, addr), val))
#define SET_SEG_MEM32(sreg, addr, val) (SET_MEM32(emu->L2P(sreg, addr), val))

// memory access(code segment)
#define GET_CODE8(offset)  (GET_SEG_MEM8(CS, offset))
#define GET_CODE16(offset) (GET_SEG_MEM16(CS, offset))
#define GET_CODE32(offset) (GET_SEG_MEM32(CS, offset))

// memory access(data segment)
#define GET_DATA8(addr)  (GET_SEG_MEM8(sreg, addr))
#define GET_DATA16(addr) (GET_SEG_MEM16(sreg, addr))
#define GET_DATA32(addr) (GET_SEG_MEM32(sreg, addr))

#define SET_DATA8(addr, val)  (SET_SEG_MEM8(sreg, addr, val))
#define SET_DATA16(addr, val) (SET_SEG_MEM16(sreg, addr, val))
#define SET_DATA32(addr, val) (SET_SEG_MEM32(sreg, addr, val))

namespace x86 {

class InsnData;
class Instruction;

const size_t REG_COUNT = 8;
const size_t SREG_COUNT= 6;

class Emulator : public EmulatorBase {
public:
	void InitInstructions();
	void InitRegisters();
	void InitMemory();
	void InitIO();
protected:
	size_t mode;
	x86::Emulator *emu = this;
	x86::InsnData *idata;
	x86::Instruction *insn16, *insn32;
	InstructionBase *insn_cache;
public:
	x86::Register32 pc;
	x86::Eflags	eflags;
	std::vector<x86::Register32> reg;
	std::vector<x86::SRegister> sreg;
	x86::MemManRegister GDTR, IDTR, TR, LDTR;
	x86::CR0_t CR0;

	bool IsMode16(){ return (mode == 16); }
	bool IsMode32(){ return (mode == 32); }

	void ChangeMode16();
	void ChangeMode32();
	void ChangeMode(){ (IsMode16() ? ChangeMode32() : ChangeMode16()); }

	inline bool IsReal(){ return (!CR0.PE); }
	inline bool IsProtected(){ return (CR0.PE); }

	void RunStep();
	void Run(bool halt_exit){
		while(!finish_flg && !(halt_exit && halt_flg)){
			if(!halt_flg)
				RunStep();
		}
	}

	struct Descriptor {
		union {
			uint32_t low32;
			struct {
				uint16_t limit_low;
				uint16_t base_low;
			};
		};

		union {
			uint32_t high32;
			struct {
				uint8_t base_mid;
				uint8_t type		: 4;
				bool S			: 1; // ディスクリプタ・タイプ(0:システム, 1:コードorデータ)
				uint8_t	DPL		: 2; // 特権レベル
				bool P			: 1; // セグメント存在
				uint8_t limit_high	: 4;
				bool AVL		: 1; // システム・ソフトウェアが使用出来る
				bool			: 1; // 虚無
				bool D_B		: 1; // Dビット(0:16bitセグメント, 1:32bitセグメント)
				bool G			: 1; // グラニュラリティ
				uint8_t base_high	: 8;
			};
		};

		uint32_t GetLimit(){
//			uint8_t high = limit_high;
//			DOUT(std::endl<<"llow=0x"<<limit_low<<", lhigh=0x"<<static_cast<uint32_t>(high)<<std::endl);
			uint32_t limit = limit_low;
			limit |= limit_high << 16;
			return limit;
		}

		uint32_t GetBase(){
			uint32_t base = base_low;
			base |= base_mid << 16;
			base |= base_high<< 24;
			return base;
		}

		std::string GetDataByString(){
			std::stringstream ss;
			ss << "base=0x"
				<< std::hex << std::setfill('0') << std::setw(8) << GetBase();
			uint64_t l_org = GetLimit();
			auto l = l_org;
			if(G) l = l << 12;
			l += (G ? 4*KB : 1);
			auto gb = l;
			auto mb = gb%GB;
			auto kb = mb%MB;
			auto  b = kb%KB;
			gb = gb/GB;
			mb = mb/MB;
			kb = kb/KB;
			ss << ", limit=0x"
				<< std::hex << std::setw(5) << l_org;

			if(!l_org || !(gb || mb || kb || b)) return ss.str();

			ss << "(" << std::dec << "G=" << G << ",size=";

			if(gb) ss << gb << "GB";
			if(mb) ss << (gb	? "-" : "") << mb << "MB";
			if(kb) ss << (gb||mb	? "-" : "") << kb << "KB";
			if(b)  ss << (gb||mb||kb? "-" : "") << b  <<  "B";

			ss << ")";

			return ss.str();
		}
	};


	inline Descriptor GetDesc(const SRegister *sreg){
		Descriptor desc;
		if(sreg->TI) throw "LDT is not implemented";

		if(sreg->index*8 >= GDTR.limit) throw "out of GDTR";
		desc.low32 = GET_MEM32(GDTR.base+(sreg->index*8));
		desc.high32= GET_MEM32(GDTR.base+(sreg->index*8)+4);
		return desc;
	}
	inline Descriptor GetDesc(const SRegister &sreg){ return GetDesc(&sreg); }

	// logical addr to physical addr
	inline uint32_t L2P(const x86::SRegister* sreg, const uint32_t &addr){
		if(!IsProtected()){ // real mode
			return (sreg->reg16 * 16) + addr;
		}

		// protect mode
//		DOUT("L2P: "<<sreg->GetName()<<"="<<sreg->GetDataByString()<<std::endl);

		// TODO: LDTはとりあえずエラー
		if(sreg->TI) throw "TI=1, LDT is not implemented.";

//		DOUT(GDTR.GetName()<<": "<<GDTR.GetDataByString()<<std::endl);

		if(sreg->index == 0x00){
			if(EFLAGS.IF){
				std::cerr<<"#GP: null selector"<<std::endl;
				throw "GP";
			}
			return addr;
		}

		Descriptor desc;

		// GDTはリニアアドレス空間にある
		if(sreg->index*8 >= GDTR.limit) throw "out of GDTR";
		desc.low32 = GET_MEM32(GDTR.base+(sreg->index*8));
		desc.high32= GET_MEM32(GDTR.base+(sreg->index*8)+4);
/*
		DOUT("desc: " << std::hex
				<< std::setw(8) << desc.low32
				<< ", 0x"
				<< std::setw(8) << desc.high32
				<< std::endl
				<< "\t"
					<< desc.GetDataByString() << std::endl);
*/

		if(addr >= desc.GetLimit()*(desc.G ? 1024 : 1)){
			debug::out_flg = true;
			DOUT("logical addr=0x"<<std::hex<<addr<<", limit=0x"<<desc.GetLimit());
			throw "out of segment";
		}
		uint32_t ret = addr + desc.GetBase();

		DOUT("phy_addr=0x"<<std::hex<<ret<<std::endl);
		return ret;
	}
	inline uint32_t L2P(const x86::SRegister &sreg, const uint32_t &addr){
		return L2P(&sreg, addr);
	}

	inline uint8_t GetCode8(int index)	{ return GET_SEG_MEM8(CS, EIP + index); }
	inline int8_t GetSignCode8(int index)	{ return static_cast<int8_t>(GetCode8(index)); }
	inline uint16_t GetCode16(int index)	{ return GET_SEG_MEM16(CS, EIP + index); }
	inline int16_t GetSignCode16(int index)	{ return static_cast<int16_t>(GetCode16(index)); }
	inline uint32_t GetCode32(int index)	{ return GET_SEG_MEM32(CS, EIP + index); }
	inline int32_t GetSignCode32(int index)	{ return static_cast<int32_t>(GetCode32(index)); }

	inline void push8(uint8_t val){
		SP--;
		SET_SEG_MEM8(SS, SP, val);
	}
	inline uint8_t pop8(){
		uint8_t val = GET_SEG_MEM8(SS, SP);
		SP++;
		return val;
	}

	inline void push16(uint16_t val){
		SP -= 2;
		SET_SEG_MEM16(SS, SP, val);
	}
	inline uint16_t pop16(){
		uint16_t val = GET_SEG_MEM16(SS, SP);
		SP += 2;
		return val;
	}

	inline void push32(uint32_t val){
		ESP -= 4;
		SET_SEG_MEM32(SS, ESP, val);
	}
	inline uint32_t pop32(){
		ESP += 4;
		return GET_SEG_MEM32(SS, ESP-4);
	}

	inline void far_jmp(uint16_t selector, uint32_t eip){
		// TODO: protect
		DOUT(std::endl<<__func__<<": CS=0x"<<std::hex<<selector<<", EIP=0x"<<eip);
		CS = selector;
		EIP = eip;

		auto desc = GetDesc(CS);
		DOUT(", desc: "<<desc.GetDataByString()<<std::endl);

		if(desc.D_B){	// 32bitセグメント
			ChangeMode32();
			DOUT("32bit-segment");
		}else{
			ChangeMode16();
			DOUT("16bit-segment");
		}
	}
};

};

#endif
