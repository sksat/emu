#ifndef EMULATOR_H_
#define EMUlATOR_H_

#include <vector>

class Emulator {
public:
	Emulator() : memory(std::vector<uint8_t>()) {}

	void load_binary(const std::string &fname);

	class CPU {
	public:
		uint32_t eip;
		uint32_t reg[8];
	} cpu;
	std::vector<uint8_t> memory;
};

#endif
