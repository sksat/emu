#include <stdio.h>
#include <sstream>
#include "memory.h"

using namespace std;

Memory::Memory(){
	mem = nullptr;
	virt_flg = false;
}

void Memory::Init(int size){
	this->size = size;
	if(mem == nullptr)
		delete[] mem;
	mem = new uint8_t[size];
	virt_flg = false;
	return;
}

int Memory::GetSize(){
	return size;
}

uint8_t Memory::operator [] (uint32_t addr) {
	if(addr > size){
		stringstream ss;
		ss<<"memory: out of range address ("<<addr<<")";
		throw ss.str();
	}
	return mem[addr];
}

void Memory::LoadBinary(const char *fname, uint32_t addr, unsigned int size){
	FILE *fp;

	uint8_t test = this->operator[](addr+size);

	fp = fopen(fname, "rb");
	if(fp == NULL) throw string("memory: LoadBinary: can't open file");
	fread(mem + addr, 1, size, fp);
	fclose(fp);
}


