#ifndef SKSAT_ENDIAN_HPP_
#define SKSAT_ENDIAN_HPP_

#include <sksat/common.hpp>

// 参考: http://clown.hatenablog.jp/entry/20090322/p1
// http://natrium11321.hatenablog.com/entry/2014/08/30/104300

namespace sksat {

class endian_base {
protected:
	int data;
	constexpr explicit endian_base(const int data) : data(data) {}
friend class endian;
public:
	constexpr bool operator==(endian_base e){ return data == e.to_int(); }
//	constexpr bool operator==(endian e){ return data == e.to_int(); }
	constexpr int to_int() const { return data; }
};

class endian : public endian_base {
friend endian_base;
public:
	endian() = default;
	constexpr explicit endian(const int data) : endian_base(data) {}
	constexpr endian(const endian_base base) : endian_base(base) {}

	constexpr bool operator==(endian_base e){
		return data == e.to_int();
	}

	const char* to_str(){
		const char *p = (*this == big) ? "big"
				: (*this == little) ? "little"
				: "unknown";
		return p;
	}

	static constexpr endian_base big{0};
	static constexpr endian_base little{1};

#if defined(__LITTLE_ENDIAN) && defined(__BIG_ENDIAN) && defined(__BYTE_ORDER)
	static constexpr endian_base defined{
	#if   (__BYTE_ORDER == __BIG_ENDIAN)
		0
	#elif (__BYTE_ORDER == __LITTLE_ENDIAN)
		1
	#endif
	};
#endif

	static endian now;

	static endian which(){
		int x = 0x00000001;
		if(*(char*)&x){ return little; }
		else return big;
	}

	static bool is_big(){    return (which() == big); }
	static bool is_little(){ return (which() == little); }
};

constexpr endian_base endian::big;
constexpr endian_base endian::little;
endian endian::now = endian::which();;

}

#endif
