#include "shell.h"
#include "command.h"

using namespace std;
using namespace shell;

streambuf* Shell::cin_buf = nullptr;
streambuf* Shell::cout_buf= nullptr;
int Shell::sh_num = 0;

void Shell::Init(Emulator *emu){
//	if(emu == nullptr) throw "shell: Emulator* is nullptr.";
	this->emu = emu;
	this->gui = nullptr;
	this->guis = vector<Gui*>();
	this->guis.push_back(nullptr);
	sh_thread = nullptr;
	ifs = nullptr;
	ofs = nullptr;
	if(sh_num == 0){
		cin_buf	= cin.rdbuf();
		cout_buf= cout.rdbuf();
	}else{
		SetDefaultStream();
	}
	script_flg = false;
	sh_num++;
}

void Shell::SetDefaultStream(){
	cin.rdbuf(cin_buf);
	cout.rdbuf(cout_buf);
	script_flg = false;
}

void Shell::ChangeStream(ifstream &ifs){
	SetDefaultStream();
	cin.rdbuf(ifs.rdbuf());
	script_flg = true;
}

void Shell::ChangeStream(ofstream &ofs){
//	SetDefaultStream();
	cout.rdbuf(ofs.rdbuf());
}

void Shell::Start(){
	if(sh_thread != nullptr){
		delete sh_thread;
	}
	sh_thread = new thread(&Shell::sh_proc, this);
}

int Shell::Exec(const char *script){
	ifstream ifs;

	ifs.open(script);
	Exec(ifs);
	return 0;
}

int Shell::Exec(ifstream &script){
	Shell *sh;
	sh = new Shell(emu);
	sh->ChangeStream(script);

	ofstream ofs;
	ofs.open("script_out.txt");
//	sh->ChangeStream(ofs);

	sh->sh_proc();
	SetDefaultStream();
//	delete sh;
	return 0;
}

struct COMMAND_DATA {
	vector<string> args;
};

string rm_space(string str){
	string tmp, ret;

	for(int i=0;i<str.size();i++){
		if(str[i] == ' ') continue;
		for(int j=i-1;j<str.size();j++){
			tmp.push_back(str[i]);
		}
		str = tmp;
		break;
	}

	for(int i=0;i<str.size()-1;i++){
		if(str[i] == ' ' && str[i+1])
			i++;
		ret.push_back(str[i]);
	}

	return ret;
}

struct ARG_INFO {
	vector<string> str;
};

vector<string> parse_line(string line){
	vector<string> arg;
	bool flg=false;
	int  now=0;
	arg.push_back(string());
	for(int i=0;i<line.size();i++){
		char c = line[i];
		if(c == ' ' || c == '\n'){
			flg = false;
//			continue;
		}else{
			flg = true;
		}
		if(flg){
			arg[now].push_back(c);
			continue;
		}else{
			now++;
			arg.push_back(string());
		}
	}

	for(int i=0;i<arg.size();i++){
		if(!arg[i].empty()) continue;
		arg.erase(arg.begin() + i);
		i--;
	}

	return arg;
}

int Shell::sh_proc(void){
try{
	InitDefaultCommand(this);
	if(!script_flg) cout<<"welcome to emulator control shell !!!"<<endl;
/*
	for(int i=0;i<cinfo.size();i++){
		cout<<cinfo[i].command<<endl;
	}
*/
	for(;;){
loop:
		string line;
		if(!script_flg) cout<<"ecsh> ";
		char c;
		for(;;){
			c = cin.get();
			if(c=='\n' || c==EOF) break;
			line.push_back(c);
		}
//		cout<<"line: "<<line<<endl;
		vector<string> args = parse_line(line);
//		if(args[0].empty()) continue;
		if(args.size() == 1){
			if(args[0].empty()){
				if(cin.eof()) break;
				cerr<<"empty"<<endl;
				continue;
			}
		}
/*
		for(int i=0;i<args.size();i++){
			cout<<"arg "<<i<<": '"<<args[i]<<"'"<<endl;
		}
*/
		bool com_flg = false;
		for(int i=0;i<cinfo.size();i++){
			if(string(cinfo[i].command) == args[0]){
//				cout<<"command: "<<args[0]<<endl;
				com_flg = true;
				cinfo[i].func(this, args);
			}
		}
		if(!com_flg){
			if(!args[0].empty())
				cout<<"コマンド \'"<<args[0]<<"\' は見つかりませんでした。"<<endl;
		}

		if(script_flg){
			if(cin.eof()) break;
		}
	}

	if(!script_flg)
		cout<<"end shell."<<endl;

}catch(string msg){
	cout<<"error: shell: "<<msg<<endl;
	return -1;
}catch(const char *msg){
	cout<<"error: shell: "<<msg<<endl;
	return -1;
}catch(...){
	cout<<"exception"<<endl;
	return -1;
}
}


