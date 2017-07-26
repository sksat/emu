#ifndef X86_REGISTER_H_
#define X86_REGISTER_H_

#include <sstream>
#include "../../register_base.h"

namespace x86 {

class Register : public ::RegisterBase {
public:
	Register() : RegisterBase(sizeof(uint32_t)) {}
	explicit Register(uint8_t r) : RegisterBase(sizeof(uint8_t)), low8(r) {}
	explicit Register(uint16_t r): RegisterBase(sizeof(uint16_t)), reg16(r){}
	explicit Register(uint32_t r): RegisterBase(sizeof(uint32_t)), reg32(r){}

	std::string GetDataByString(){
		std::stringstream ss;
		ss<<"0x"<<std::hex<<reg32;
		return ss.str();
	}

	operator uint8_t () { return low8;			}
	operator int8_t  () { return (int8_t)low8;	}
	operator uint16_t() { return reg16;			}
	operator int16_t () { return (int16_t)reg16;}
	operator uint32_t() { return reg32;			}
	operator int32_t () { return (int32_t)reg32;}

	union {
		uint32_t reg32;
		uint16_t reg16;
		struct {
			uint8_t low8;
			uint8_t high8;
		};
	};
};


};

#endif
