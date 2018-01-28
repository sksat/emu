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

/*
class Register8 : public ::RegisterBase {
public:
	Register8() : ::RegisterBase(sizeof(uint8_t)), reg8(0x00) {}
	explicit Register8(uint8_t r) : ::RegisterBase(sizeof(uint8_t)), reg8(r) {}

	inline operator uint8_t () { return reg8; }
	inline operator int8_t  () { return reg8; }

	inline virtual Register8& operator=(const uint8_t data){
		reg8 = data;
		return *this;
	}

	inline virtual Register8& operator++(int){
		reg8++;
		return *this;
	}

	inline uint8_t Get8() const{ return reg8; }
	inline void Set8(uint8_t r){ reg8 = r; }

	virtual const std::string GetDataByString(){
		std::stringstream ss;
		ss << "0x"
			<< std::hex << std::setw(2) << std::setfill('0')
			<< static_cast<uint32_t>(reg8);
			return ss.str();
	}

protected:
	uint8_t reg8;
};

class Register16 : public Register8 {
//public ::RegisterBase {
public:
	Register16() : high8(0x00) { SetSize(sizeof(uint16_t)); }
	Register16(uint16_t r) : high8(r >> 8) { SetSize(sizeof(uint16_t)); Set8((uint8_t)r);}

	inline operator uint16_t () { return ((high8 << 8) | reg8); }
	inline operator  int16_t () { return ((high8 << 8) | reg8); }
	inline operator uint32_t () { return static_cast<uint32_t>(static_cast<uint16_t>(*this)); }
	inline operator  int32_t () { return static_cast< int32_t>(static_cast<uint16_t>(*this)); }

//	inline Register16& operator=(const uint8_t data){
//		reg8 = data;
//		return *this;
//	}

	inline virtual Register16& operator=(const uint16_t data) {
		high8 = data >> 8;
		reg8  = (uint8_t)data;
		return *this;
	}

	template<typename T>
        inline Register16& operator+=(T diff){
		Set16(Get16()+diff);
		return *this;
	}

	inline virtual Register16& operator++(int){
		Set16(Get16()+1);
		return *this;
	}

	inline uint16_t Get16() const {
		return ((high8 << 8) | reg8);
	}

	inline void Set16(uint16_t r){
		high8 = r >> 8;
		reg8  = (uint8_t)r;
	}

	virtual const std::string GetDataByString(){
		std::stringstream ss;
		ss  << "0x"
			<< std::hex << std::setw(4) << std::setfill('0')
			<< this->Get16();
		return ss.str();
	}

protected:
//	Register8 low8, high8;
	uint8_t high8;
};


class Register32 : public Register16 {
public:
	Register32(){ SetSize(sizeof(uint32_t)); }
	Register32(uint32_t r) : high16(r >> 16) { SetSize(sizeof(uint32_t)); Set16((uint16_t)r); }

	inline operator uint32_t () { return ((high16 << 16) | Get16()); }

//	inline operator Register16 () {
//		return Register16(Get16());
//	}

	inline Register32& operator=(uint32_t r){
		Set32(r);
		return *this;
	}

	inline virtual Register32& operator=(const Register16 &r16){
		Set16(r16.Get16());
		return *this;
	}

	template<typename T>
	inline Register32& operator+=(T diff){
		Set32(Get32()+diff);
		return *this;
	}

	template<typename T>
	inline Register32& operator-=(T diff){
		Set32(Get32()-diff);
		return *this;
	}

	inline virtual Register32& operator++(int){
		Set32(Get32()+1);
		return *this;
	}

	template<typename T>
	inline bool operator==(T val) const {
		return Get32() == static_cast<uint32_t>(val);
	}

	inline uint32_t Get32() const {
		return ((high16 << 16) | Get16());
	}

	inline void Set32(uint32_t r){
		high16 = r >> 16;
		Set16((uint16_t)r);
	}

	const std::string GetDataByString(){
		std::stringstream ss;
		ss << "0x"
			<< std::hex
			<< std::setw(8)
			<< std::setfill('0')
			<< this->Get32();
		return ss.str();
	}
protected:
//	Register16 low16, high16;
	uint16_t high16;
};

*/

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

	/*
	inline void UpdateSub(uint32_t v1, uint32_t v2, uint64_t res){
		int sign1 = v1 >> 31;
		int sign2 = v2 >> 31;
		int signr = (res >> 31) & 1;

		SetCarry(res >> 32);
		SetZero(res == 0);
		SetSign(signr);
		SetOverflow(sign1 != sign2 && sign1 != signr);
	}
*/

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

/*
template<typename T>
uint32_t operator+(const Register32 &reg, const T &val){
	return reg.reg32. + val;
}

template<typename T>
uint32_t operator+(T val, Register32 reg32){
	return val + reg32.Get32();
}

template<typename T>
uint32_t operator-(Register32 reg32, T val){
	return reg32.Get32() - val;
}

template<typename T>
uint32_t operator-(T val, Register32 reg32){
	return val - reg32.Get32();
}
*/

}

#endif
