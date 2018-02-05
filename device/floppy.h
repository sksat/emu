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

	void Reset(){
		Setting set = {
			.drive = 0,
			.head = 0,
			.cylinder = 0,
			.sector = 1
		};
		Seek(set);
	}
	bool Seek(const Setting &set);
	bool Read(Memory *mem, uint32_t addr, uint8_t sector_num);
	bool Read(const Setting &set, Memory *mem, uint32_t addr, uint8_t sector_num){
		if(!Seek(set)) return false;
		if(!Read(mem, addr, sector_num)) return false;
		return true;
	}

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
