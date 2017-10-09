#ifndef FONT_H_
#define FONT_H_

#include <vector>
#include "../common.h"

class Font {
public:
	Font(const char *fname);
	~Font();

	int GetNum(){ return num; }

	bool LoadFile(const char *fname);

	inline char operator[](int i){
		return data[i];
	}
private:
	int xsize, ysize, num;
	std::vector<char> data;
};

#endif
