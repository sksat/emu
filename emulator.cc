#include <iostream>
#include <fstream>
#include <filesystem>
#include "emulator.h"

void Emulator::load_binary(const std::string &fname){
	using namespace std::filesystem;

	std::cerr << "loading \""+fname+"\"..." << std::endl;

	if(!exists(fname))
		throw std::runtime_error("\""+fname+"\" does not exist.");
	auto fsize = file_size(fname);
	std::cerr<<"file size: "<<fsize<<std::endl;
	memory.resize(fsize);

	auto file = std::fstream(fname, std::ios::in | std::ios::binary);
	file.read(reinterpret_cast<char*>(&memory[0]), fsize);
}
