#include "shell.h"
#include "command.h"

using namespace std;
using namespace shell;

streambuf* Shell::cin_buf = nullptr;
streambuf* Shell::cout_buf= nullptr;
int Shell::sh_num = 0;

void Shell::Init(Emulator *emu){
	if(emu == nullptr) throw "shell: Emulator* is nullptr.";
	this->emu = emu;
	sh_thread = nullptr;
	ifs = nullptr;
	ofs = nullptr;
	if(sh_num == 0){
		cin_buf	= cin.rdbuf();
		cout_buf= cout.rdbuf();
	}else{
		SetDefaultStream();
	}
	sh_num++;
}

void Shell::SetDefaultStream(){
	cin.rdbuf(cin_buf);
	cout.rdbuf(cout_buf);
}

void Shell::ChangeStream(ifstream &ifs){
	SetDefaultStream();
	cin.rdbuf(ifs.rdbuf());
}

void Shell::ChangeStream(ofstream &ofs){
	SetDefaultStream();
	cout.rdbuf(ofs.rdbuf());
}

void Shell::Start(){
	if(sh_thread != nullptr){
		delete sh_thread;
	}
	sh_thread = new thread(&Shell::sh_proc, this);
}

void Shell::sh_proc(void){
	InitDefaultCommand(this);
	cout<<"welcome to emulator control shell !!!"<<endl;
	string str;
	string com;
	for(;;){
		cout<<"$ ";
		cin>>str;
		cout<<"test: "<<str<<endl;
		com = str;
		for(int i=0; i<cinfo.size(); i++){
			if(cinfo[i].command == com){
				cout<<"command: "<<com<<endl;
			}
		}
	}
}


