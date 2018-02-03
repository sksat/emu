#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "device.h"

#define DEFAULT_SCRNX	320
#define DEFAULT_SCRNY	200

//#define hankaku _binary_hankaku_bin_start 

//extern char hankaku[4096];	// /font/hankaku.txt フォントのバイナリをロードするべき

namespace Device {

class Display : public Device::Base {
public:
	Display();
	~Display();

	void Init();
	void InitDevName(){ name = "display"; }

	void MemoryMappedProc(Memory *memory, uint32_t addr){ throw "display: MemoryMappedProc"; }

	void ChangeMode(size_t scrnx, size_t scrny, bool txtmode_flg);
	void ChangeMode(size_t scrnx, size_t scrny){ ChangeMode(scrnx, scrny, txtmode_flg); }

	unsigned char* GetImage(){ return img; }
	void FlushImage();

	void TestDraw();

	unsigned char vram[0xffff];	// とりあえず0xa0000 ~ 0affff だけ考える
private:
	bool txtmode_flg;
	unsigned char pallete[0xff * 3]; // パレット
	unsigned char *img;		// ウィンドウに実際に渡すイメージ
	size_t scrnx, scrny;	// 縦、横サイズ

	// text mode
	char *font;
	size_t font_xsiz, font_ysiz;
};

}

#endif
