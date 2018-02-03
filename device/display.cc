#include <iostream>
#include <fstream>
#include "display.h"

using namespace Device;

size_t Display::default_scrnx = 320;
size_t Display::default_scrny = 200;

Display::Display() : img(nullptr), scrnx(default_scrnx), scrny(default_scrny) {
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

#define POS(x, y) ((y*scrnx + x)*0x3)
#define SET_RGB(x, y, r, g, b) { img[POS(x,y)]=r; img[POS(x,y)+1]=g; img[POS(x,y)+2]=b; }
#define FONT_DATA(c,y) (font[c*font_ysiz + y+4])

void Display::FlushImage(){
	if(txtmode_flg){
		size_t x = 0;
		size_t y = 0;
		for(unsigned int i=0;i<0xffff;i++){
			char c = vram[i]; // 文字

			switch(c){
			case '\n':
			case EOF:
			case '\0':
				goto not_print;
			default:
				std::cout<<std::hex<<(uint32_t)c<<":"<<c<<std::endl;
				break;
			}

			for(size_t fy=0;fy<font_ysiz;fy++){
				unsigned char d = FONT_DATA(c, fy);
				uint8_t l = 0x80;
				for(size_t fx=0;fx<font_xsiz;fx++){
					if((d & l) != 0x00){
						SET_RGB(x+fx, y+fy, 0xff, 0xff, 0xff);
					}
					l = l / 2;
				}
			}
/*
			uint8_t *font_data = &font[c*font_ysiz];
			for(size_t j=0;j<font_ysiz;j++){
				uint8_t d = font_data[j];
				unsigned int l = 0x80;
				for(size_t k=0;k<font_xsiz;k++){
					if((d & l) != 0){
						unsigned int addr = x+k+(y+j)*scrnx;
						img[addr*3] = 0xff;
					}
					l = l/2;
				}
			}
*/
not_print:
			x += font_xsiz;
			if(x>scrnx){
				x = 0;
				y += font_ysiz;
			}
			if(y>scrny) break;
		}
	}
	TestDraw();
}

void Display::TestDraw(){
	vram[0] = 'a';
	vram[1] = 'b';
	vram[2] = 'c';
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

