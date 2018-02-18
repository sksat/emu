#ifndef DEVICE_KEYBOARD_H_
#define DEVICE_KEYBOARD_H_

#include "device.h"

namespace Device {

class Keyboard : public Device::Base {
public:
	Keyboard() : Base("Keyboard") {}

	uint8_t in8(const uint16_t &port){
		DOUT(std::endl<<"Keyboard::in8 port="<<std::hex<<port<<" is implemented temporary"<<std::endl);
		return 0x00;
	}

	void out8(const uint16_t &port, const uint8_t &val){
		DOUT(std::endl<<"Keyboard::out8 port="<<std::hex<<port<<", val="<<static_cast<uint32_t>(val)<<" is implemented temporary"<<std::endl);
	}
};

}

#endif
