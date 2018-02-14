#ifndef BIOS_JUNK_X86_H_
#define BIOS_JUNK_X86_H_

#include <iostream>
#include <typeinfo>
#include "junk_base.h"
#include "../arch/x86/emulator.h"
#include "../device/floppy.h"
#include "../device/display.h"

// http://oswiki.osask.jp/?(AT)BIOS

namespace BIOS {
namespace Junk {

class x86 : public BIOS::Junk::Base {
public:
	x86(::x86::Emulator *e) : BIOS::Junk::Base(e), emu(e), disp(nullptr) {}

	void SetDisplay(Device::Display &disp){ this->disp = &disp; }
	void Print(const std::string &str){
		if(this->disp !=nullptr){
			disp->Print(str);
			return;
		}
		DOUT("BIOS: Print: ");
		std::cout<<str;
		DOUT(std::endl);
	}
	void Print(const char c){
		if(this->disp != nullptr){
			disp->Print(c);
			return;
		}
		DOUT("BIOS: Print: ");
		std::cout<<c;
		DOUT(std::endl);
	}

	void Boot() {
		DOUT("booting x86 BIOS..."<<std::endl);

		std::string msg = "x86 junk BIOS(version: ";
		msg += GIT_COMMIT_ID;
		msg += ")\n\n";
		Print(msg);

		LoadIPL();

		EIP = 0x7c00;
		ESP = 0x7c00;
	}

	Device::Floppy* GetFloppy(){
		auto& dev = emu->dev;
		for(size_t i=0;i<dev.size();i++){
			auto& d = *dev[i];
			if(typeid(d) == typeid(Device::Floppy))
				return dynamic_cast<Device::Floppy*>(dev[i]);
		}
		return nullptr;
	}

	Device::Base* GetBootDevice(){
		auto fd = GetFloppy();
		if(fd != nullptr) return fd;
		Print("No bootable device.\n");
		emu->halt_flg = true;
		return nullptr;
	}

	void LoadIPL(){
		auto d = GetBootDevice();
		if(d == nullptr) return;
		if(typeid(*d) == typeid(Device::Floppy)){
			Print("Booting from Floppy...\n");
			Device::Floppy *f = dynamic_cast<Device::Floppy*>(d);
			f->Load(emu->memory, 0x7c00, 256);
		}else{
			Print("unknown bootable device.\n");
		}
	}

	void Function(size_t num){
		DOUT("\n\tBIOS function called: num = 0x"<<std::hex<<num<<std::endl);
		switch(num){
		case 0x10:
			video_func();
			break;
		case 0x13:
			disk_func();
			break;
		case 0x16:
			keyboard_func();
			break;
		default:
			{
				std::stringstream ss;
				ss << "not implemented: BIOS function 0x"
					<< std::hex << num;
				throw ss.str();
			}
			break;
		}
	}

	void video_func(){
		DOUT("\tvideo function  ");
		DOUT("AH = 0x"<<std::hex<<static_cast<uint32_t>(AH)<<std::endl);
//		if(disp == nullptr) throw "junk BIOS: Display is null";
		switch(AH){
		case 0x00:
			{
				std::string desc;
				size_t xsize, ysize, bit;
				bool txt;
				switch(AL){
				case 0x13:
					desc  = "VGA graphics";
					txt   = false;
					xsize = 320;
					ysize = 200;
					bit = 8;
					break;
				default:
					throw "not implemented: video mode(junk BIOS)";
					break;
				}

				if(disp != nullptr)
					disp->ChangeMode(xsize, ysize, txt);
				DOUT("VIDEO MODE: "<<desc<<" "
						<<std::dec<<xsize<<"x"<<ysize<<"x"
						<<bit<<"bit");
			}
			break;
		case 0x0e:
#ifdef DEBUG
			std::cout <<"BIOS function putchar: " << AL << std::endl;
#endif
			Print(static_cast<const char>(AL));
			break;
		default:
			throw "not implemented: video_func(junk BIOS)";
			break;
		}
	}

	void disk_func(){
		auto fd = GetFloppy();
		switch(AH){
		case 0x00:	// システムのリセット
			fd->Reset();
			break;
		case 0x02:	// 読み込み
		//case 0x03:	// 書き込み
		//case 0x04:	// ベリファイ
		//case 0x0c:	// シーク
			{
				// CH=シリンダ番号 & 0xff  <=シリンダ番号下位8bit
				// CL=セクタ番号(0-5bit) | (シリンダ番号 & 0x300) >> 2
				// CL:0-5bit=セクタ番号
				// CL:6-7bit=シリンダ番号9,10bit目
				// 0xff  = 0000,1111,1111
				// 0x300 = 0011,0000,0000

				// 本当はシリンダ番号の指定は以下のようになるけど，
				// フロッピーでは0x00～0x4fだから9,10bit目はいらないのでCHだけでok
				//uint16_t cyl = CH | ((CL << 2) & 0x300)
				Device::Floppy::Setting set = {
					.drive = DL,
					.head  = (DH==0) ? bool(0) : bool(1),
					.cylinder = CH,
					.sector = CL // 下位5bit
				};
				if(fd->Read(set, emu->memory, (ES*16)+BX, AL)){
					EFLAGS.CF = 0;
					AH = 0;
				}else{
					EFLAGS.CF = 1;
					AH = 0; // TODO: エラーコードわからん
				}
			}
			break;
		//case 0x08:	// パラメータ取得
		//case 0x09:	// パラメータテーブルの初期化(?)
		//case 0x10:	// ドライブレディーのテスト
		//case 0x11:	// ドライブの正規化
		//case 0x14:	// コントローラの診断
		//case 0x15:	// ディスクタイプ取得
		//case 0x16:	// FDの交換チェック
		//case 0x17:	// FDのディスクタイプの設定
		//case 0x05:	// FDの物理フォーマット
		default:
			{
				std::stringstream ss;
				ss << "not implemented: BIOS disk func AH=0x"
					<<std::hex<<static_cast<uint32_t>(AH);
				throw ss.str();
			}
			break;
		}
	}

	void keyboard_func(){
		switch(AH){
		//case 0x00:	// 文字読み出し
		//case 0x01:	// キーバッファ状態チェック
		case 0x02:	// キーロック＆シフト状態取得
			// TODO: temporary impl
			DOUT("warning: BIOS keyboard func AH=0x02 is implemented temporary.");
			AL = 0x00;
			break;
		default:
			{
				std::stringstream ss;
				ss << "not implemented: BIOS keyboard func AH=0x"
					<< std::hex << static_cast<uint32_t>(AH);
				throw ss.str();
			}
		}
	}
private:
	::x86::Emulator *emu;
	Device::Display *disp;
};

}
}

#endif
