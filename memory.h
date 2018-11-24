#ifndef MEMORY_H_
#define MEMORY_H_

#include <cstdint>
#include <vector>

class Memory {
private:
	std::vector<uint8_t> mem;
public:
	using addr_t = uint32_t;

	inline size_t get_size() const {
		return mem.size();
	}
	inline void set_size(const size_t size){
		mem.resize(size);
	}
	void load_binary(const addr_t &addr, const std::string &fname);

// memory access
	inline uint8_t get8(const addr_t &addr) const {
		return mem[addr];
	}
	inline void set8(const addr_t &addr, const uint8_t &val){
		mem[addr] = val;
	}

	// 16bit access(little endian)
	inline uint16_t get16(const addr_t &addr) const {
		uint16_t ret = 0x00;
		for(auto i=0;i<2;i++)
			ret |= get8(addr + i) << (i*8);
		return ret;
	}
	inline void set16(const addr_t &addr, const uint16_t &val){
		for(auto i=0;i<2;i++)
			set8(addr+i, val>>(i*8));
	}

	// 32bit access(little endian)
	inline uint32_t get32(const addr_t &addr) const {
		uint32_t ret = 0x00;
		for(auto i=0;i<4;i++)
			ret |= get8(addr + i) << (i*8);
		return ret;
	}
	inline void set32(const addr_t &addr, const uint32_t &val){
		for(auto i=0;i<4;i++)
			set8(addr+i, val>>(i*8));
	}
};

#endif
