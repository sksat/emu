#ifndef DEVICE_FLOPPY_H_
#define DEVICE_FLOPPY_H_

#include <cstdio>
#include <sys/stat.h>

#include "device.h"
#include "../memory.h"

namespace Device {

class Floppy : public Device::Base { // block deviceのクラスを作った方が良い？
public:
	Floppy() : fp(nullptr) {}
	Floppy(const char *fname) : fp(nullptr) { SetFile(fname); }

	~Floppy(){
		if(fp != nullptr)
			fclose(fp);
	}

	void InitDevName(){
		name = "floppy";
	}

	void SetFile(const char *fname){
		struct stat st;
		if(stat(fname, &st) != 0)
			throw "No such floppy file";
		if(fp != nullptr)
			throw "already opend";
		fp = fopen(fname, "rb");
		if(fp == nullptr)
			throw "could not open image file.";
	}

	void Load(Memory *mem, uint32_t addr, size_t size){
		mem->LoadBinary(fp, addr, size);
	}
private:
	FILE *fp;
};

}

#endif
