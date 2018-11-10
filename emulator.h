#ifndef EMULATOR_H_
#define EMUlATOR_H_

#include <vector>

class Emulator {
public:
	Emulator() : memory(std::vector<uint8_t>()) {}

	void load_binary(const std::string &fname);

	std::vector<uint8_t> memory;
};

#endif
