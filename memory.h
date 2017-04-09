#ifndef MEMORY_H_
#define MEMORY_H_

#include <stdint.h>
#include <vector>
#include "device/device.h"

class Device;

struct MapInfo {
	uint32_t addr;
	uint32_t end_addr;
	Device *dev;
	uint8_t *mem;
};

class Memory {
public:
	Memory();
	~Memory();

	void Init(int size);
	int GetSize();

	bool IsVirt() { return virt_flg; }
	void EnableVirt(){ virt_flg=true; }
	void UnableVirt(){ virt_flg=false; }

	uint8_t operator [] (uint32_t addr);

	void MapDevice(Device *dev, uint32_t addr, unsigned int size);
	void MapMemory(uint8_t *mem, uint32_t addr, unsigned int size);

	void LoadBinary(const char *fname, uint32_t addr, unsigned int size);

	void Dump(const char *fname, uint32_t addr, unsigned int size);
private:
	unsigned int size;
	uint8_t *mem;
	std::vector<MapInfo> minfo;
	int minfo_num;
	bool virt_flg;
};

#endif
