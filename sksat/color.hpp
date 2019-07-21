#ifndef SKSAT_COLOR_HPP_
#define SKSAT_COLOR_HPP_

#include <sksat/common.hpp>

namespace sksat {

class rgb24 {
public:
	rgb24() : r(0x00), g(0x00), b(0x00) {}
	rgb24(uint8_t r, uint8_t g, uint8_t b){ set(r,g,b); }

	void set(uint8_t r, uint8_t g, uint8_t b){
		this->r = r;
		this->g = g;
		this->b = b;
	}

	uint8_t r,g,b;
};

class rgba32 : public rgb24 {
public:
	rgba32(): a(0x00) {}
	rgba32(uint8_t r, uint8_t g, uint8_t b, uint8_t a){ set(r,g,b,a); }

	void set(uint8_t r, uint8_t g, uint8_t b, uint8_t a){ rgb24::set(r,g,b); this->a = a; }

	rgba32& operator=(const rgb24 &rgb){
		set(rgb.r, rgb.g, rgb.b, 0x00);
		return *this;
	}

	uint8_t a;
};


using rgb  = rgb24;
using rgba = rgba32;

using color= rgb;

}

#endif
