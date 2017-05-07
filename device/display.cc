#include "display.h"

Display::Display(){
	InitDevice();
	img = nullptr;
}

Display::~Display(){
	if(img != nullptr) delete[] img;
}

void Display::Init(){
	txtmode_flg = true;
	font = hankaku;
	font_xsiz = 8;
	font_ysiz = 16;
	ChangeMode(DEFAULT_SCRNX, DEFAULT_SCRNY);
}

void Display::ChangeMode(unsigned int scrnx, unsigned int scrny, bool txtmode_flg){
	if(img != nullptr) delete[] img;
	img = new unsigned char[scrnx*scrny*3];
	this->scrnx = scrnx;
	this->scrny = scrny;
}

void Display::FlushImage(){
	if(txtmode_flg){
		unsigned int char_xnum = 0xffff * font_xsiz / scrnx;
		int x=0, y=0;
		for(unsigned int i=0;i<0xffff;i++){
			char c = vram[i];
			char *font_data = font+c*font_ysiz;
			for(int j=0;j<font_ysiz;j++){
				char d = font_data[j];
				unsigned int l = 0x80;
				for(int k=0;k<font_xsiz;k++){
					if((d & l) != 0){
						int addr = x+k+(y+j)*scrnx;
						img[addr*3] = 0xff;
					}
					l = l/2;
				}
			}
			x += font_xsiz;
			if(x>scrnx){
				x = 0;
				y += font_ysiz;
			}
		}
	}
}

void Display::TestDraw(){
	vram[0] = 'a';
	vram[1] = 'b';
	vram[2] = 'c';
	for(int y=100;y<=150;y++){
		for(int x=20;x<=50;x++){
			int p = (y*scrnx + x)*3;
			img[p  ] = 0xff;
			img[p+1] = 0xff;
			img[p+2] = 0x00;
		}
	}
}

