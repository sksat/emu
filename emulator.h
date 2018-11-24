#ifndef EMULATOR_H_
#define EMUlATOR_H_

#include <vector>
#include "cpu.h"
#include "memory.h"

class Emulator {
public:
	void load_binary(const std::string &fname);
	void dump_registers();

	CPU cpu;
	Memory memory;
};

#endif
