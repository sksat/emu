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
	img = new unsigned char[scrnx*scrny];
}

void Display::FlushImage(){
	if(txtmode_flg){
		
	}else{
		
	}
}
