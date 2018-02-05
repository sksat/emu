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
	Base() : name("unknown device") {}

	virtual void MemoryMappedProc(Memory *memory, uint32_t addr){}
	const std::string& GetDevName(){ return name; }
protected:
	virtual void InitDevice() { InitDevName(); InitDevIRQ(); InitDevPort(); }
	virtual void InitDevName() = 0;
	void InitDevIRQ() { irq		= std::vector<int>(); }
	void InitDevPort(){ io_port	= std::vector<int>(); }
	void SetDevName(const std::string &name){ this->name = name; }
	void AddDevIRQ(int irq){ this->irq.push_back(irq); }
	void AddDevPort(int port){ io_port.push_back(port); }

	std::string name;
	std::vector<int> irq;
	std::vector<int> io_port;
};

};

#endif
