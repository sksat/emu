#ifndef REGISTER_H_
#define REGISTER_H_

#include <cstdint>

struct Register {
	explicit Register() : r32(0x00) {}
	union {
		uint32_t r32;
		struct {
			union {
				uint16_t r16;
				struct {
					uint8_t r8;
					uint8_t h8;
				};
			};
			uint16_t h16;
		};
	};
};

struct SRegister {
	explicit SRegister() : r16(0x00) {}
	union {
		uint16_t r16;
		struct {
			uint8_t	 RPL	: 2;
			uint8_t  TI		: 1;
			uint16_t index	:13;
		};
	};
};

// EFLAGS
struct Eflags {
	explicit Eflags() : r32(0x00) {}
	union {
		uint32_t r32;
		struct {
			bool CF : 1;
			bool _r1: 1;
			bool PF : 1;
			bool _r2: 1;
			bool AF : 1;
			bool _r3: 1;
			bool ZF : 1;
			bool SF : 1;
			bool TF : 1;
			bool IF : 1;
			bool DF : 1;
			bool OF : 1;
			uint8_t IOPL : 2;
			bool NT : 1;
			bool _r4: 1;
			bool RF : 1;
			bool VM : 1;
			bool AC : 1;
			bool VIF: 1;
			bool VIP: 1;
			bool ID : 1;
		};
	};

	inline bool check_parity(const uint8_t &b) const {
		bool p = true;
		for(auto i=0;i<8;i++)
			p ^= (b>>i) & 1;
		return p;
	}

	template<typename T>
	inline T update_add(const T &v1, const uint32_t &v2){
		uint64_t result = static_cast<uint64_t>(v1) + v2;
		auto size = sizeof(T)*8; // bit数
		bool s1 = v1 >> (size-1);
		bool s2 = v2 >> (size-1);
		bool sr = (result >> (size-1)) & 1;

		CF = result >> size;
		PF = check_parity(result & 0xff); // 下位1byte
		ZF = !result;
		SF = sr;
		OF = s1 != s2 && s1 != sr;
		return static_cast<T>(result);
	}

	template<typename T>
	inline T update_sub(const T &v1, const uint32_t &v2){
		uint64_t result = static_cast<uint64_t>(v1) - v2;
		auto size = sizeof(T)*8;
		bool s1 = v1 >> (size-1);
		bool s2 = v2 >> (size-1);
		bool sr = (result >> (size-1)) & 1;

		CF = result >> size;
		PF = check_parity(result & 0xff);
		ZF = !result;
		SF = sr;
		OF = s1 != s2 && s1 != sr;
		return static_cast<T>(result);
	}

	template<typename T>
	inline T update_and(const T &v1, const uint32_t &v2){
		uint64_t result = static_cast<uint64_t>(v1) & v2;
		auto size = sizeof(T)*8;
		bool sr = (result >> (size-1)) & 1;

		CF = false;
		PF = check_parity(result & 0xff);
		ZF = !result;
		SF = sr;
		OF = false;
		return static_cast<T>(result);
	}

	template<typename T>
	inline T update_or(const T &v1, const uint32_t &v2){
		uint64_t result = static_cast<uint64_t>(v1) | v2;
		auto size = sizeof(T)*8;
		bool sr = (result >> (size-1)) & 1;

		CF = false;
		PF = check_parity(result & 0xff);
		ZF = !result;
		SF = sr;
		OF = false;
		return static_cast<T>(result);
	}

	template<typename T>
	inline void update_cmp(const T &v1, const uint32_t &v2){
		update_sub(v1, v2);
	}
};

#endif
