#ifndef DEVICE_FLOPPY_H_
#define DEVICE_FLOPPY_H_

#include <fstream>

#include "device.h"
#include "../memory.h"

// http://softwaretechnique.jp/OS_Development/kernel_development11.html

namespace Device {

class Floppy : public Device::Base {
public:
	Floppy(){}
	Floppy(const std::string fname) : fname(fname) { Open(fname); }
	~Floppy(){}

	void InitDevName(){ name = "floppy"; }

	void Open(const std::string &fname);
	void Open(){ Open(fname); }

	struct Setting {
		uint8_t drive;
		bool head;		// 0(表), 1(裏)
		uint16_t cylinder;	// 0〜79
		uint8_t sector : 5;	// 1〜18
	};

	bool Seek(const Setting &set);
	bool Read(Memory *mem, uint32_t addr, uint8_t sector_num);

	void SetFile(const char *fname){
		throw "not implemented: SetFile";
/*		struct stat st;
		if(stat(fname, &st) != 0)
			throw "No such floppy file";
		if(fp != nullptr)
			throw "already opend";
		fp = fopen(fname, "rb");
		if(fp == nullptr)
			throw "could not open image file.";
*/
	}

	void Load(Memory *mem, uint32_t addr, uint32_t size);
//		throw "not implemented: Load";
//		mem->LoadBinary(fp, addr, size);
//	}
private:
	std::string fname;
	std::fstream fs;
	Setting set;
};

}

#endif
