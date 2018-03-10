#include <iostream>
#include <fstream>
#include <sstream>
#include "display.h"

using namespace Device;

size_t Display::default_scrnx = 320;
size_t Display::default_scrny = 200;

#define SET_PALETTE(num, r, g, b) {palette[num*3]=r;palette[num*3+1]=g;palette[num*3+2]=b;}

Display::Display() : Base("Display"), memory(nullptr), img(nullptr), scrnx(default_scrnx), scrny(default_scrny) {
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
	SET_PALETTE(15, 0xff, 0xff, 0xff);
}

void Display::out8(const uint16_t &port, const uint8_t &data){
	static uint16_t old_port = 0x00;
	static uint8_t p_num = 0;
	static uint8_t col = 0;

	switch(port){
	case 0x03c8:
		p_num = data;
		break;
	case 0x03c9:
		if(old_port == port) col++;
		if(col >= 3) col = 0;
		palette[p_num*3 + col] = data;
		std::cout<<"palette: num="<<std::dec<<p_num<<", col="<<(col==0 ? "Red" : (col==1 ? "Green" : "Blue"))<<std::endl;
		break;
	default:
		std::stringstream ss;
		ss << "not implemented port=0x" << std::hex
			<< port << " in " << GetDevName();
		throw ss.str();
	}

	old_port = port;
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
	this->txtmode_flg = txtmode_flg;
	Clear();
}

#define POS(x, y) (((y)*scrnx + x)*3)
#define SET_RGB(x, y, r, g, b) { img[POS(x,y)]=r; img[POS(x,y)+1]=g; img[POS(x,y)+2]=b; }
#define FONT_DATA(c,y) (font[c*font_ysiz + y+5])
#define GET_VRAM(addr) (memory->GetData8(vram_start+addr))
#define SET_VRAM(addr, val) { memory->SetData8(vram_start+addr, val); }

void Display::FlushImage(){
	if(memory == nullptr || img == nullptr) throw;
	size_t x=0, y=0;
	if(txtmode_flg){
		for(size_t addr=0x00; addr<=vram_size; addr++){
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

			PutFont(print_x, print_y, '_');
		}
	}else{
		for(size_t addr=0x00; addr<vram_size; addr++){
			uint8_t num = GET_VRAM(addr); // 色番号
			uint8_t r = palette[num * 3    ];
			uint8_t g = palette[num * 3 + 1];
			uint8_t b = palette[num * 3 + 2];
			SET_RGB(x, y, r, g, b);

			x++;
			if(x>=scrnx){
				x = 0;
				y++;
			}
		}
	}
}

void Display::Clear(){
	for(size_t y=0;y<scrny;y++){
		for(size_t x=0;x<scrnx;x++)
			SET_RGB(x, y, 0x00, 0x00, 0x00);
	}
}

void Display::PutFont(size_t x, size_t y, char c, uint8_t r, uint8_t g, uint8_t b){
	if(c == ' '){
		for(size_t fy=0;fy<font_ysiz;fy++){
			for(size_t fx=0;fx<font_xsiz;fx++)
				SET_RGB(x+fx, y+fy, 0x00, 0x00, 0x00);
		}
	}
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

