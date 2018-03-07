#ifndef DEVICE_H_
#define DEVICE_H_

#include <stdint.h>
#include <vector>
#include <string>
#include "../common.h"
#include "../memory.h"

class Memory;

namespace Device {

class Base {
public:
	Base(const std::string &name) : name(name) {}
//	Base() : name("unknown device") {}

	virtual void MemoryMappedProc(Memory *memory, uint32_t addr){}

	const std::string& GetDevName(){ return name; }

	virtual uint8_t in8(const uint16_t &port){
		std::string msg = "in8() is not implemented in ";
		msg += name;
		throw msg;
	}
	virtual void out8(const uint16_t &port, const uint8_t &data){
		std::string msg = "out8() is not implemented in ";
		msg += name;
		throw msg;
	}

//	virtual void Init() { InitDevName(); }//InitDevIRQ(); InitDevPort(); }
//	virtual void InitDevName() = 0;
//	void InitDevIRQ() { irq		= std::vector<int>(); }
//	void InitDevPort(){ io_port	= std::vector<int>(); }
//	void AddDevIRQ(int irq){ this->irq.push_back(irq); }
//	void AddDevPort(int port){ io_port.push_back(port); }

protected:
	std::string name;
	std::vector<int> irq;
	std::vector<int> io_port;
};

};

#endif
