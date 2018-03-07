#ifndef DEVICE_PIC_H_
#define DEVICE_PIC_H_

#include "device.h"

namespace Device {

class PIC : public Device::Base {
public:
	PIC() : Base("PIC(master)") {}
	PIC(PIC *master) : Base("PIC(slave)") {}

	void out8(const uint16_t &port, const uint8_t &data){
		DOUT(std::endl<<name<<": out8() port=0x"<<std::hex<<static_cast<uint32_t>(port)
				<<", data=0x"<<static_cast<uint32_t>(data));
	}
private:
	// registers
	uint8_t imr;
	uint8_t icw1, icw2, icw3, icw4;
};

}

#endif
