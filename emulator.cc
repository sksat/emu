#include <iostream>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include "emulator.h"
#include "util.h"

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

void Emulator::dump_registers(){
	using std::endl;
	std::cout
		<< "----- dump registers-----" << endl
		<< "EIP: " << hex2str(EIP) << endl
		<< "EAX: " << hex2str(EAX) << endl
		<< "ECX: " << hex2str(ECX) << endl
		<< "EDX: " << hex2str(EDX) << endl
		<< "EBX: " << hex2str(EBX) << endl
		<< "ESP: " << hex2str(ESP) << endl
		<< "EBP: " << hex2str(EBP) << endl
		<< "ESI: " << hex2str(ESI) << endl
		<< "EDI: " << hex2str(EDI) << endl
		<< "-------------------------" << std::endl;
}
