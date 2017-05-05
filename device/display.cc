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
		
	}else{
		
	}
}

void Display::TestDraw(){
	for(int y=0;y<=100;y++){
		for(int x=0;x<=50;x++){
			int p = (y*scrnx + x)*3;
			img[p  ] = 0xff;
			img[p+1] = 0xff;
			img[p+2] = 0x00;
		}
	}
}
