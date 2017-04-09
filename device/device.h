#ifndef DEVICE_H_
#define DEVICE_H_

#include <stdint.h>
#include "../memory.h"

class Memory;

class Device {
public:
	virtual void MemoryMappedProc(Memory *memory, uint32_t addr) = 0;
};

#endif
