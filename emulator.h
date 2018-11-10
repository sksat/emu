#ifndef EMULATOR_H_
#define EMUlATOR_H_

#include <vector>
#include "cpu.h"

class Emulator {
public:
	Emulator() : memory(std::vector<uint8_t>()) {}

	void load_binary(const std::string &fname);
	void dump_registers();

	CPU cpu;
	std::vector<uint8_t> memory;
};

#endif
