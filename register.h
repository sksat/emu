#ifndef REGISTER_H_
#define REGISTER_H_

#include <stdint.h>

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

class Flags : Register {
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

#endif
