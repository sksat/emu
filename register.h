#ifndef REGISTER_H_
#define REGISTER_H_

#include <stdint.h>

/*
class Register {
public:
	union {
		uint64_t reg64;
		uint32_t reg32;
		uint16_t reg16;
		struct {
			uint8_t low8;
			uint8_t high8;
		};
	};
};

class Flags : public Register {
public:
	bool getCarry();
	bool getZero();
	bool getSign();
	bool getTrap();
	bool getInt();
	bool getDirect();
	bool getOverflow();
	bool getVirtInt();
private:
	int mode;
};
*/

class Register {
public:
	Register(){}
	explicit Register(size_t size){}
	Register(uint8_t r) : low8(r) {}
	Register(uint16_t r): reg16(r){}
	Register(uint32_t r): reg32(r){}

	void SetSize(size_t size){}

// とりあえずサイズのチェックはしない
	uint8_t Get8()		{ return low8; }
	int8_t GetSign8()	{ return (int8_t)Get8(); }
	uint8_t GetHigh8()	{ return high8; }
	int8_t GetSignHign8(){return (int8_t)GetHigh8();}
	uint16_t Get16()	{ return reg16;}
	int16_t GetSign16()	{ return (int16_t)Get16(); }
	uint32_t Get32()	{ return reg32; }
	int32_t GetSign32()	{ return (int32_t)Get32(); }

//private:
public:
	size_t size;
	union {
		uint32_t reg32;
		uint16_t reg16;
		struct {
			uint8_t low8;
			uint8_t high8;
		};
	};
};

#endif
