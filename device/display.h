#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "device.h"

#define DEFAULT_SCRNX	320
#define DEFAULT_SCRNY	200

extern char hankaku[4096];	// /font/hankaku.txt

class Display : public Device {
public:
	Display();
	~Display();

	void Init();

	void MemoryMappedProc(Memory *memory, uint32_t addr){ throw "display: MemoryMappedProc"; }

	void ChangeMode(unsigned int scrnx, unsigned int scrny, bool txtmode_flg);
	void ChangeMode(unsigned int scrnx, unsigned int scrny){ ChangeMode(scrnx, scrny, txtmode_flg); }

	unsigned char* GetImage(){ return img; }
	void FlushImage();

	void TestDraw();

	unsigned char vram[0xffff];	// とりあえず0xa0000 ~ 0affff だけ考える
private:
	bool txtmode_flg;
	unsigned char pallete[0xff * 3]; // パレット
	unsigned char *img;		// ウィンドウに実際に渡すイメージ
	unsigned int scrnx, scrny;	// 縦、横サイズ
};

#endif
