#include <stdio.h>
#include <iostream>
#include <sstream>
#include "memory.h"

using namespace std;

Memory::Memory():size(0){
	mem = nullptr;
	virt_flg = false;
}

Memory::~Memory(){
	delete[] mem;
}

void Memory::Init(int s){
//	this->size = size;
//	if(mem == nullptr)
//		delete[] mem;
	mem = new uint8_t[s];
	virt_flg = false;
	return;
}

int Memory::GetSize(){
	return size;
}

uint8_t Memory::operator [] (uint32_t addr) {
	if(addr > (uint32_t)size){
		stringstream ss;
		ss<<"memory: out of range address ("<<hex<<showbase<<addr<<")";
		throw ss.str();
	}
	uint8_t ret = mem[addr];

	if(minfo.empty()) return ret;
	for(int i=0;i<minfo.size();i++){
		if(addr < minfo[i].addr)	continue;
		if(addr > minfo[i].end_addr)	continue;
		if(minfo[i].mem != nullptr){
			ret = minfo[i].mem[addr - minfo[i].addr];
			return ret;
		}
		if(minfo[i].dev != nullptr){
			minfo[i].dev->MemoryMappedProc(this, addr);
			return ret;
		}
	}

	return ret;
}

void Memory::MapDevice(Device *dev, uint32_t addr, unsigned int size){
//	throw "Memory: MapDevice() not implemented.";
	MapInfo mi;
	mi.dev  = dev;
	mi.mem  = nullptr;
	mi.addr = addr;
	mi.end_addr = addr + size;
	minfo.push_back(mi);
}

void Memory::MapMemory(uint8_t *mem, uint32_t addr, unsigned int size){
//	throw "Memory: MapMemory() not implemented.";
	MapInfo mi;
	mi.dev  = nullptr;
	mi.mem  = mem;
	mi.addr = addr;
	mi.end_addr = addr + size;
	minfo.push_back(mi);
}

void Memory::LoadBinary(const char *fname, uint32_t addr, unsigned int size){
	FILE *fp;

//	uint8_t test = this->operator[](addr+size);

	cout<<"loading binary: "<<fname<<endl;

	fp = fopen(fname, "rb");
	if(fp == NULL) throw "memory: LoadBinary: can't open file";
	fread(mem + addr, 1, size, fp);
	fclose(fp);
}

void Memory::Dump(const char *fname, uint32_t addr, unsigned int size){
	uint8_t test = this->operator[](addr+size);
	FILE *fp;
	fp = fopen(fname, "wb");
	if(fp == NULL) throw "memory: Dump: can't open file.";
	fwrite(mem+addr, sizeof(uint8_t), size, fp);
	fclose(fp);
}

