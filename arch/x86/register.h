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

	const std::string GetDataByString() const {
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
	union {
		uint16_t reg16;
		struct { // segment selector
			uint8_t	 RPL	: 2; // required priviledge level
			bool     TI	: 1; // table indicator
			uint16_t index	: 13;
		};
	};

	inline SRegister& operator=(const uint16_t val){ reg16 = val; return *this; }

	const std::string GetDataByString() const {
		std::stringstream ss;
		ss	<< "0x"
			<< std::hex << std::setfill('0')
			<< std::setw(4) << reg16
			<< " = ("
			<< "RPL=0x" << static_cast<uint16_t>(RPL)
			<< ", TI=0x" << TI << ":" << (TI ? "LDT" : "GDT")
			<< ", index=0x" << std::setw(4) << index
			<< ")";
		return ss.str();
	}
};

// EFLAGS
class Eflags : public ::RegisterBase {
public:
	union {
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
		uint32_t reg32;
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

	template<typename T>
	inline void UpdateOr(T v1, uint32_t v2, uint64_t result){
		auto size = sizeof(T)*8;
		bool signr = (result >> (size-1)) & 1;

		SetCarry(0);
		SetZero(result == 0);
		SetSign(signr);
		SetOverflow(0);
	}

	template<typename T>
	inline void UpdateShr(T v1, uint8_t v2, uint64_t result){
		if(v2 == 0) return;

		auto size = sizeof(T)*8;
		bool signr = (result >> (size-1)) & 1;

		SetCarry((v1 >> (v2-1)) & 1);
		SetZero(result == 0);
		SetSign(signr);

		// OFは1bitシフトのときのみ影響を受ける
		if(v2 == 1)
			SetOverflow((v1>>(size-1)) & 1);
	}

	template<typename T>
	inline void UpdateSal(T dest, uint8_t count, uint64_t result){
		if(count == 0) return;

		auto size = sizeof(T)*8;
		bool signr = (result >> (size-1)) & 1;

		SetCarry((result>>size) & 1);
		SetZero(result == 0);
		SetSign(signr);

		// OFは1bitシフトのときのみ影響を受ける
		if(count == 1)
			SetOverflow(CF != ((result>>(size-1)) & 1));
	}

	inline void UpdateXor(){
		CF = OF = 0;
	}

	const uint32_t GetData32() const {
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

	const std::string GetDataByString() const {
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
	MemManRegister() : ::RegisterBase(sizeof(uint64_t)), limit(0x00), base(0x00), selector(0x00) {}

	union {
		struct {
			uint16_t limit		: 16;
			uint32_t base		: 32;
			uint16_t selector	: 16;	// TR, LDTRのみ
		};
//		uint64_t _reg64 : 64;
	};

	const std::string GetDataByString() const {
		std::stringstream ss;

		if(GetSize() != sizeof(uint64_t) && selector != 0x00)
				throw "selector is not zero";

		ss	<< std::hex
			<< std::setfill('0');

		if(GetSize()==sizeof(uint64_t))
			ss << "0x" << std::setw(4) << selector;
		else
			ss << "    0x";

		ss	<< std::setw(8) << base
			<< std::setw(4) << limit;

		ss	<< " = (";

		if(GetSize() == sizeof(uint64_t))
			ss << "selector=0x" << std::setw(4) << selector << ", ";

		ss	<< "base=0x" << std::setw(8) << base
			<< ", limit=0x" << std::setw(4) << limit
			<< ")";
		return ss.str();
	}
};

// base for CR0~CR4
class CRegister : public ::RegisterBase {
public:
	CRegister() : ::RegisterBase(sizeof(uint32_t)) {}
};

class CR0_t : public CRegister {
public:
	union {
		struct {
			bool PE  : 1; // 保護イネーブル
			bool MP  : 1; // モニタ・コプロセッサ
			bool EM  : 1; // エミュレーション
			bool TS  : 1; // タスクスイッチ
			bool ET  : 1; // 拡張タイプ
			bool NE  : 1; // 数値演算エラー
			uint16_t : 10;
			bool WP  : 1; // 書き込み保護
			bool     : 1;
			bool AM  : 1; // アラインメント・マスク
			uint16_t : 10;
			bool NW  : 1; // ノット・ライトスルー
			bool CD  : 1; // キャッシュ・ディスエーブル
			bool PG  : 1; // ページング
		};
		uint32_t reg32;
	};

	const std::string GetDataByString() const {
		std::stringstream ss;
		ss << "0x"
			<< std::hex
			<< std::setfill('0')
			<< std::setw(8)
			<< reg32;
		return ss.str();
	}
};

}

#endif
