#ifndef DEVICE_H_
#define DEVICE_H_

#include <stdint.h>
#include <vector>
#include "../memory.h"

class Memory;

class Device {
public:
	virtual void MemoryMappedProc(Memory *memory, uint32_t addr) = 0;
	const char* GetDevName(){ return dev_name; }
protected:
	void InitDevice() { InitDevName(); InitDevIRQ(); InitDevPort(); }
	void InitDevName(){ dev_name	= (const char*)"unknown device"; }
	void InitDevIRQ() { irq		= std::vector<int>(); }
	void InitDevPort(){ io_port	= std::vector<int>(); }
	void SetDevName(const char *name){ dev_name = name; }
	void AddDevIRQ(int irq){ this->irq.push_back(irq); }
	void AddDevPort(int port){ io_port.push_back(port); }
private:
	const char *dev_name;
	std::vector<int> irq;
	std::vector<int> io_port;
};

#endif
