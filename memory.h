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
	uint8_t operator [] (uint32_t addr){
		return GetData8(addr);
	}
	//int8_t operator [] (uint32_t addr){ return static_cast<int8_t>(operator=(addr)); }
//	uint32_t operator[](uint32_t addr);

	uint8_t  GetData8(uint32_t addr);

	uint32_t GetData32Big(uint32_t addr){ throw "big endian"; }
	uint32_t GetData32Little(uint32_t addr);
	uint32_t GetData32(uint32_t addr){
		if(endian == ENDIAN::BIG)	return GetData32Big(addr);
		return GetData32Little(addr);
	}

	void SetData8(uint32_t addr, uint32_t val){
		mem[addr] = val;
	}
	void SetData32Big(uint32_t addr, uint32_t val){ throw "big endian"; }
	void SetData32Little(uint32_t addr, uint32_t val);
	void SetData32(uint32_t addr, uint32_t val){
		if(endian == ENDIAN::BIG){
			SetData32Big(addr, val);
		}else{
			SetData32Little(addr, val);
		}
		return;
	}

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
