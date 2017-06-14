#ifndef MEMORY_H_
#define MEMORY_H_

#include <stdint.h>
#include <vector>
#include "device/device.h"

enum class ENDIAN { BIG, LITTLE, };

class Device;

struct MapInfo {
	uint32_t addr;
	uint32_t end_addr;
	Device *dev;
	uint8_t *mem;
};

class Memory {
public:
	ENDIAN endian;
	int size;

	Memory();
	~Memory();

	void Init(int size);
	int GetSize();

	bool IsVirt() { return virt_flg; }
	void EnableVirt(){ virt_flg=true; }
	void UnableVirt(){ virt_flg=false; }

	Memory* operator->(){ return this; }

	// little endianでのuint32_tの読み込み処理はEmulatorクラスから持ってくる
	uint8_t operator [] (uint32_t addr);

	void MapDevice(Device *dev, uint32_t addr, unsigned int size);
	void MapMemory(uint8_t *mem, uint32_t addr, unsigned int size);

	void LoadBinary(const char *fname, uint32_t addr, unsigned int size);

	void Dump(const char *fname, uint32_t addr, unsigned int size);
private:
	uint8_t *mem;
	std::vector<MapInfo> minfo;
	int minfo_num;
	bool virt_flg;
};

#endif
