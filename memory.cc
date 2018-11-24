#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "memory.h"
#include "util.h"

void Memory::load_binary(const Memory::addr_t &addr, const std::string &fname){
	using namespace std::filesystem;

	std::cerr << "loading \""+fname+"\"..." << std::endl;
	if(!exists(fname))
		throw std::runtime_error("\""+fname+"\" does not exist");
	
	auto fsize = file_size(fname);
	std::cerr << "file size: " << fsize << std::endl;

	if(addr+fsize > this->get_size()){
		this->set_size(addr+fsize);
	}

	auto file = std::fstream(fname, std::ios::in | std::ios::binary);
	file.read(reinterpret_cast<char*>(&mem[addr]), fsize);
}
