#include <stdio.h>
#include <iostream>
#include <sstream>
#include "memory.h"

using namespace std;

Memory::Memory():size(0x00){
	virt_flg = false;
}

Memory::~Memory(){}

void Memory::Init(int size){
	this->size = size;
	if(mem.size() == 0)
		mem.clear();
	mem.resize(size);
	virt_flg = false;
	minfo.clear();
	return;
}

int Memory::GetSize(){
	return size;
}

uint8_t Memory::GetData8(uint32_t addr) {
	if(addr > static_cast<uint32_t>(size)){
		stringstream ss;
		ss<<"memory: out of range address ("<<hex<<showbase<<addr<<")";
		throw ss.str();
	}
	uint8_t ret = mem[addr];

//	std::cout<<"minfo num ="<<minfo.size()<<std::endl;

	if(minfo.empty()) return ret;
	for(unsigned int i=0;i<minfo.size();i++){
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

uint32_t Memory::GetData32Little(uint32_t addr){
	uint32_t ret = 0;
	for(int i=0;i<4;i++){
		ret |= GetData8(addr + i) << (i*8);
	}
	return ret;
}

/*
uint32_t Memory::operator[](uint32_t addr){
	if(endian == ENDIAN::LITTLE){
		return GetData32Little(addr);
	}else{
		throw "big endian";
	}
}
*/

void Memory::SetData32Little(uint32_t addr, uint32_t val){
	for(int i=0;i<4;i++)
		SetData8(addr+i, val>>(i*8));
	return;
}

void Memory::MapDevice(Device::Base *dev, uint32_t addr, unsigned int size){
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

void Memory::LoadBinary(FILE *fp, uint32_t addr, size_t size){
	if(fp == nullptr) throw "FILE* is nullptr";
//	fread(mem + addr, 1, size, fp);
	cout<<"fread: "<<"size: "<<size<<std::endl;
	fread(&mem[addr], sizeof(uint8_t), size, fp);
}

void Memory::LoadBinary(const char *fname, uint32_t addr, size_t size){
	FILE *fp;

//	uint8_t test = this->operator[](addr+size);

	cout<<"loading binary from file: \""<<fname<<"\""<<endl;

	fp = fopen(fname, "rb");
	if(fp == nullptr) throw "memory: LoadBinary: can't open file";
	LoadBinary(fp, addr, size);
	fclose(fp);
}

void Memory::Dump(const char *fname, uint32_t addr, unsigned int size){
	uint8_t test = this->operator[](addr+size);
	FILE *fp;
	fp = fopen(fname, "wb");
	if(fp == NULL) throw "memory: Dump: can't open file.";
	fwrite(&mem[addr], sizeof(uint8_t), size, fp);
	fclose(fp);
}

