#include <iostream>
#include <sstream>
#include "floppy.h"

using namespace Device;

void Floppy::Open(const std::string &fname){
	std::stringstream ss;
	fs.open(fname, std::ios::in | std::ios::out |std::ios::binary);
	if(fs.fail()){
		ss << "Floppy: cannot open file \'"
			<<fname
			<<"\'";
		throw ss.str();
	}

	// ファイルサイズのチェック
	size_t size;
	{
		fs.seekg(0, std::fstream::end);
		auto end = fs.tellg();
		fs.clear();
		fs.seekg(0, std::fstream::beg);
		auto start = fs.tellg();
		size = end - start;
	}
	DOUT("Floppy: file size="<<size/KB<<"KB"<<std::endl);
	if(size/KB != 1440) throw "Floppy: file size is not 1440KB";
}

bool Floppy::Seek(const Floppy::Setting &set){
	if(set.cylinder > 79){
		DOUT("Floppy: cylinder error: "<<static_cast<uint32_t>(set.cylinder)<<std::endl);
		return false;
	}
	if(set.sector==0 || set.sector > 18){
		DOUT("Floppy: sector error: "<<static_cast<uint32_t>(set.sector)<<std::endl);
		return false;
	}

	DOUT("Floppy: Seek: drive="<<std::dec<<static_cast<uint32_t>(set.drive)
			<<", head="<<set.head
			<<", cylinder="<<static_cast<uint32_t>(set.cylinder)
			<<", sector="<<static_cast<uint32_t>(set.sector)
			<<std::endl);
	this->set = set;

	fs.seekg(0, std::fstream::beg);
	if(set.head)
		fs.seekg(512*18, std::fstream::cur);
	if(set.cylinder)
		fs.seekg(set.cylinder*512*36, std::fstream::cur);
	fs.seekg(512*(set.sector-1), std::fstream::cur);
//	std::cout<<set.head*18 + set.cylinder*36 + (set.sector -1)<<std::endl;
	if(fs.fail()) return false;
	return true;
}

bool Floppy::Read(Memory *mem, uint32_t addr, uint8_t sector_num){
	DOUT("Floppy: Read: sector num="<<static_cast<uint32_t>(sector_num)<<std::endl);
	Load(mem, addr, 512*sector_num);
	return true;
}

void Floppy::Load(Memory *mem, uint32_t addr, uint32_t size){
	DOUT("Floppy: loading to memory... addr=0x"<<std::hex<<addr<<", size=0x"<<size<<std::endl);
	fs.read((char*)&mem->GetRaw()[addr], size);
}
