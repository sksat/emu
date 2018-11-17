#ifndef UTIL_H_
#define UTIL_H_

#include <string>
#include <sstream>
#include <iomanip>

template<typename T>
const std::string hex2str(const T &val, int n=32/8){
	std::stringstream ss;
	ss << "0x" << std::hex << std::setw(n*2) << std::setfill('0')
		<< static_cast<uint32_t>(val);
	return ss.str();
}

#endif
