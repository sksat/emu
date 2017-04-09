#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "device.h"

extern char hankaku[4096];

class Display : public Device {
public:
	void FlushImage();
private:
	bool txtmode_flg;
	unsigned char pallete[0xff * 3]; // パレット
	unsigned char *img;		// ウィンドウに実際に渡すイメージ
	unsigned int scrnx, scrny;	// 縦、横サイズ
};

#endif
