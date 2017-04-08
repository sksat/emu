#ifndef MEMORY_H_
#define MEMORY_H_

#include <stdint.h>

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

	void LoadBinary(const char *fname, uint32_t addr, unsigned int size);
private:
	unsigned int size;
	uint8_t *mem;
	bool virt_flg;
};

#endif
