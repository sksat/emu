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
};

}

#endif
