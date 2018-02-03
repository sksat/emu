#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "device.h"

namespace Device {

class Display : public Device::Base {
public:
	Display();
	~Display();

	void Init();
	void InitDevName(){ name = "display"; }

	void LoadFont(const std::string &fname);
	void RegisterVRAM(Memory *mem, const uint32_t addr, const uint32_t size){
		this->memory = mem;
		vram_start = addr;
		vram_size = size;
	}

	void MemoryMappedProc(Memory *memory, uint32_t addr){ throw "display: MemoryMappedProc"; }

	void ChangeMode(size_t scrnx, size_t scrny, bool txtmode_flg);
	void ChangeMode(size_t scrnx, size_t scrny){ ChangeMode(scrnx, scrny, txtmode_flg); }

	unsigned char* GetImage(){ return img; }
	void FlushImage();

	void PutFont(size_t x, size_t y, char c, uint8_t r, uint8_t g, uint8_t b);
	void PutFont(size_t x, size_t y, char c){ PutFont(x, y, c, 0xff, 0xff, 0xff); }
	void Print(const std::string &str);

	void TestDraw();

	size_t GetSizeX() const { return scrnx; }
	size_t GetSizeY() const { return scrny; }

	static size_t default_scrnx, default_scrny;
private:
//	unsigned char vram[0xffff];	// とりあえず0xa0000 ~ 0affff だけ考える
	Memory* memory;
	uint32_t vram_start, vram_size;

	bool txtmode_flg;
	unsigned char pallete[0xff * 3]; // パレット
	unsigned char *img;		// ウィンドウに実際に渡すイメージ
	size_t scrnx, scrny;	// 縦、横サイズ

	// text mode
	std::vector<uint8_t> font;
	size_t font_xsiz, font_ysiz;
};

}

#endif
