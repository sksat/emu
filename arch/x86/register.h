#ifndef X86_REGISTER_H_
#define X86_REGISTER_H_

#include <iomanip>
#include <sstream>
#include "../../register_base.h"

namespace x86 {

class Register32 : public ::RegisterBase {
public:
	Register32() : ::RegisterBase(sizeof(uint32_t)), reg32(0x00) {}
	explicit Register32(uint32_t val) : ::RegisterBase(sizeof(uint32_t)), reg32(val) {}
	union {
		uint32_t reg32;
		struct {
			union {
				uint16_t reg16;
				uint16_t low16;
				struct {
					union {
						uint8_t reg8;
						uint8_t low8;
					};
					uint8_t high8;
				};
			};
			uint16_t high16;
		};
	};

//	inline operator uint32_t () { return reg32; }
//	inline operator uint16_t () { return reg16; }
//	inline operator uint8_t  () { return reg8;  }

	inline Register32& operator=(const uint32_t val){ reg32 = val; return *this; }

	const std::string GetDataByString(){
		std::stringstream ss;
		ss << "0x"
			<< std::hex << std::setw(8) << std::setfill('0')
			<< static_cast<uint32_t>(reg32);
			return ss.str();
	}
};

class SRegister : public ::RegisterBase {
public:
	SRegister() : ::RegisterBase(sizeof(uint16_t)), reg16(0x00) {}
	uint16_t reg16;

	inline SRegister& operator=(const uint16_t val){ reg16 = val; return *this; }

	const std::string GetDataByString(){
		std::stringstream ss;
		ss << "0x"
			<< std::hex << std::setw(4) << std::setfill('0')
			<< static_cast<uint32_t>(reg16);
			return ss.str();
	}
};

// EFLAGS
class Eflags : public ::RegisterBase {
public:
	struct {
		bool CF : 1;
		bool    : 1;
		bool PF : 1;
		bool    : 1;
		bool AF : 1;
		bool    : 1;
		bool ZF : 1;
		bool SF : 1;
		bool TF : 1;
		bool IF : 1;
		bool DF : 1;
		bool OF : 1;
		bool IOPL1 : 1;
		bool IOPL2 : 1;
		bool NT : 1;
		bool    : 1;
		bool RF : 1;
		bool VM : 1;
		bool AC : 1;
		bool VIF: 1;
		bool VIP: 1;
		bool ID : 1;
	};
public:
	Eflags() : RegisterBase(sizeof(uint32_t)) {}

	inline bool IsCarry()		{ return CF; }
	inline bool IsParity()		{ return PF; }
	inline bool IsZero()		{ return ZF; }
	inline bool IsSign()		{ return SF; }
	inline bool IsOverflow()	{ return OF; }
	inline bool IsInterrupt()	{ return IF; }
	inline bool IsDirection()	{ return DF; }

	inline void SetCarry(bool carry)	{ CF = carry; }
	inline void SetParity(bool parity)	{ PF = parity; }
	inline void SetZero(bool zero)		{ ZF = zero; }
	inline void SetSign(bool sign)		{ SF = sign; }
	inline void SetOverflow(bool of)	{ OF = of; }
	inline void SetInterrupt(bool intr)	{ IF = intr; }
	inline void SetDirection(bool dir)	{ DF = dir; }

	template<typename T>
	inline void UpdateSub(T v1, uint32_t v2, uint64_t result){
		auto size = sizeof(T)*8; // bit数
		bool sign1 = v1 >> (size-1);
		bool sign2 = v2 >> (size-1);
		bool signr = (result >> (size -1)) & 1; // (res >> 31) & 1;

		SetCarry(result >> size);
		SetZero(result == 0);
		SetSign(signr);
		SetOverflow(sign1 != sign2 && sign1 != signr);
	}

	template<typename T>
	inline void Cmp(T v1, uint32_t v2){
		uint64_t result = static_cast<uint64_t>(v1) - static_cast<uint64_t>(v2);
		UpdateSub(v1, v2, result);
	}

	template<typename T>
	inline void UpdateAdd(T v1, uint32_t v2, uint64_t result){
		UpdateSub(v1, v2, result);
	}

	template<typename T>
	inline void UpdateAnd(T v1, uint32_t v2, uint64_t result){
		auto size = sizeof(T)*8; // bit数
		bool signr = (result >> (size-1)) & 1;

		SetCarry(0);
		SetZero(result == 0);
		SetSign(signr);
		SetOverflow(0);
	}

	const uint32_t GetData32(){
		uint32_t ret = 0x00;
		ret |= CF;
		ret |= PF	<< 2;
		ret |= AF	<< 4;
		ret |= ZF	<< 6;
		ret |= SF	<< 7;
		ret |= TF	<< 8;
		ret |= IF	<< 9;
		ret |= DF	<< 10;
		ret |= OF	<< 11;
		ret |= IOPL1<< 12;
		ret |= IOPL2<< 13;
		ret |= NT	<< 14;
		ret |= RF	<< 16;
		ret |= VM	<< 17;
		ret |= AC	<< 18;
		ret |= VIF	<< 19;
		ret |= VIP	<< 20;
		ret |= ID	<< 21;
		return ret;
	}

	const std::string GetDataByString(){
		std::stringstream ss;
		ss	<< "0x"
			<< std::hex
			<< std::setw(8)
			<< std::setfill('0')
			<< GetData32();
		return ss.str();
	}
};

// memory management register: GDTR, IDTR, TR, LDTR
class MemManRegister : public ::RegisterBase {
public:
	MemManRegister() : ::RegisterBase(sizeof(uint64_t)), selector(0x00), base(0x00), limit(0x00) {}

	union {
		struct {
			uint16_t selector;	// TR, LDTRのみ
			uint32_t base;
			uint16_t limit;
		};
		uint64_t _reg64;
	};

	const std::string GetDataByString(){
		std::stringstream ss;
		ss	<< "0x"
			<< std::hex
			<< std::setw(GetSize()*2)
			<< std::setfill('0')
			<< _reg64;
		return ss.str();
	}
};

}

#endif
