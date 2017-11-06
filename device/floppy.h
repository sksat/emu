#ifndef DEVICE_FLOPPY_H_
#define DEVICE_FLOPPY_H_

#include <stdio.h>
#include <sys/stat.h>

#include "device.h"

class Floppy : public Device { // block deviceのクラスを作った方が良い？
public:
	Floppy() : fp(nullptr) {}
	Floppy(const char *fname) : fp(nullptr) { SetFile(fname); }

	~Floppy(){
		if(fp != nullptr)
			fclose(fp);
	}

	void InitDevName(){
		dev_name = (const char*)"floppy";
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
private:
	FILE *fp;
};

#endif
