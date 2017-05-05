#ifndef SHELL_H_
#define SHELL_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include "../emulator.h"
#include "../gui/gui.h"

class Shell;
class Emulator;

typedef int command_func_t(Shell* sh, std::vector<std::string> args);

struct CommandInfo {
	const char *command;
	command_func_t *func;
};

class Shell {
public:
	Shell(Emulator *emu){ Init(emu); }
	Shell(Emulator *emu, std::ifstream &ifs, std::ofstream &ofs){
		Init(emu);
		ChangeStream(ifs, ofs);
	}
	~Shell(){ Close(); }
	void Init(Emulator *emu);
	void SetDefaultStream();
	void ChangeStream(std::ifstream &ifs);
	void ChangeStream(std::ofstream &ofs);
	void ChangeStream(std::ifstream &ifs, std::ofstream &ofs){
		ChangeStream(ifs);
		ChangeStream(ofs);
	}
	void Start();
	void Stop();
	void Close();
	void Register(const char *com, command_func_t* f){
		CommandInfo i;
		i.command = com;
		i.func    = f;
		cinfo.push_back(i);
	}
	int Exec(const char *script);
	int Exec(std::ifstream &script);

	int exec_command(std::string);

	Emulator* get_emu(){ return emu; }
	void set_emu(Emulator *emu){ this->emu = emu; }
	Gui* get_gui(){ return gui; }
	Gui* get_gui(int n){
		if(n<guis.size()) return guis[n];
		return nullptr;
	}
	int get_guinum(){
		for(int i=1;i<guis.size();i++){
			if(guis[i] == gui)
				return i;
		}
		return 0;
	}
	void set_gui(Gui *gui){
		this->gui = gui;
		guis.push_back(gui);
	}
//private:
	int sh_proc(void);
private:
	Emulator *emu;
	Gui *gui;
	std::vector<Gui*> guis;
	std::thread *sh_thread;
	std::vector<CommandInfo> cinfo;
	std::ifstream *ifs;
	std::ofstream *ofs;
	bool script_flg;
	static std::streambuf *cin_buf, *cout_buf;
	static int sh_num;
};

#endif
