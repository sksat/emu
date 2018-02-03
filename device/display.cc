#include <iostream>
#include <fstream>
#include "display.h"

using namespace Device;

size_t Display::default_scrnx = 320;
size_t Display::default_scrny = 200;

Display::Display() : memory(nullptr), img(nullptr), scrnx(default_scrnx), scrny(default_scrny) {
	InitDevice();
	Init();
}

Display::~Display(){
	if(img != nullptr) delete[] img;
}

void Display::Init(){
	txtmode_flg = true;
	font_xsiz = 8;
	font_ysiz = 16;
	ChangeMode(scrnx, scrny);
}

void Display::LoadFont(const std::string &fname){
	std::cout<<"loading font file: \""<<fname<<"\""<<std::endl;
	std::ifstream file(fname, std::ios::binary);
	if(!file){
		std::string msg = __func__;
		msg += ": can't open file.";
		throw msg;
	}
	file.seekg(0, std::ios::end);
	auto size = file.tellg();
	file.clear();
	file.seekg(0, std::ios::beg);
	font.resize(size);
	file.read((char*)&font[0], size);
}

void Display::ChangeMode(size_t scrnx, size_t scrny, bool txtmode_flg){
	if(img != nullptr) delete[] img;
	img = new unsigned char[scrnx*scrny*3];
	this->scrnx = scrnx;
	this->scrny = scrny;
}

#define POS(x, y) (((y)*scrnx + x)*3)
#define SET_RGB(x, y, r, g, b) { img[POS(x,y)]=r; img[POS(x,y)+1]=g; img[POS(x,y)+2]=b; }
#define FONT_DATA(c,y) (font[c*font_ysiz + y+5])
#define GET_VRAM(addr) (memory->GetData8(vram_start+addr))
#define SET_VRAM(addr, val) { memory->SetData8(vram_start+addr, val); }

void Display::FlushImage(){
	if(memory == nullptr || img == nullptr) throw;
	if(txtmode_flg){
		size_t x = 0;
		size_t y = 0;
		for(size_t addr=0x00; addr<(vram_start+vram_size); addr++){
			char c = GET_VRAM(addr); // 文字

			switch(c){
			case '\n':
			case EOF:
			case '\0':
				break;
			default:
				PutFont(x, y, c);
				break;
			}

			x += font_xsiz;
			if(x>scrnx){
				x = 0;
				y += font_ysiz;
			}
			if(y>scrny) break;
		}
	}
}

void Display::PutFont(size_t x, size_t y, char c, uint8_t r, uint8_t g, uint8_t b){
	for(size_t fy=0;fy<font_ysiz;fy++){
		unsigned char d = FONT_DATA(c, fy);
		uint8_t l = 0x80;
		for(size_t fx=0;fx<font_xsiz;fx++){
			if((d & l) != 0x00) SET_RGB(x+fx, y+fy, r, g, b);
			l = l / 2;
		}
	}
}


void Display::Print(char c){
	switch(c){
	case '\n':
		print_x = 0;
		print_y += font_ysiz;
		return;
	case '\t':
		print_x += 4*font_xsiz;
		return;
	default:
		PutFont(print_x, print_y, c);
		break;
	}

	print_x += font_xsiz;
	if(print_x > scrnx){
		print_x = 0;
		print_y += font_ysiz;
	}
	if(print_y > scrny) print_y = 0;
}

void Display::Print(const std::string &str){
	for(size_t i=0;i<str.size();i++){
		char c = str[i];
		Print(c);
	}
}

void Display::TestDraw(){
	Print("a\n\nbbbbbbccccc\tddddde\n\tfffggghijklmnopq\nrstuvwxyz!");
//	SET_VRAM(0, 'a');
//	SET_VRAM(1, 'b');
//	SET_VRAM(2, 'c');
/*
	for(int y=100;y<=150;y++){
		for(int x=20;x<=50;x++){
			int p = (y*scrnx + x)*3;
			img[p  ] = 0xff;
			img[p+1] = 0xff;
			img[p+2] = 0x00;
		}
	}
*/
}

