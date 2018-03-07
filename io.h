#ifndef IO_H_
#define IO_H_

#include <unordered_map>
#include "device/device.h"

class IO {
public:
	IO(){}
	~IO(){}

	using Port = uint16_t;
	std::unordered_map<Port, Device::Base*> port;

	bool check(const Port &p){
		if(port.find(p) != port.end()) return true;
		DOUT(std::endl<<"no device connected at port:0x"<<std::hex<<p<<std::endl);
		throw "no device connected";
		return false;
	}

	uint8_t in8(const Port &p){
		check(p);
		return port[p]->in8(p);
	}

	void out8(const Port &p, const uint8_t &data){
		check(p);
		port[p]->out8(p, data);
	}
};

#endif
