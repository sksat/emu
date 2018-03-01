#ifndef REGISTER_BASE_H_
#define REGISTER_BASE_H_

#include <stdint.h>
#include <string>

/*

class Register {
public:
	Register(){}
	explicit Register(size_t size){}
	explicit Register(uint8_t r) : low8(r) {}
	explicit Register(uint16_t r): reg16(r){}
	explicit Register(uint32_t r): reg32(r){}

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

	operator uint8_t()	{ return Get8(); }
	operator int8_t()	{ return GetSign8(); }
	operator uint16_t()	{ return Get16(); }
	operator int16_t()	{ return GetSign16(); }
	operator uint32_t()	{ return Get32(); }
	operator int32_t()	{ return GetSign32(); }

	std::string name;

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

*/

class RegisterBase {
public:
	explicit RegisterBase(std::size_t s) : size(s) {};

	std::size_t GetSize() const	{ return size; }
	void SetSize(std::size_t s)	{ size = s; }

	const std::string GetName() const { return name; }
	void SetName(std::string n) { name = n; }
	void SetName(const char* n) { name = n; }

	virtual const std::string GetDataByString() const = 0;
private:
	std::size_t size;
	std::string name;
};

#endif
