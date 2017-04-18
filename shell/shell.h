#ifndef SHELL_H_
#define SHELL_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include "../emulator.h"

class Shell;
class Emulator;

typedef int command_func_t(Shell* sh, Emulator *emu, std::vector<std::string> args);

struct CommandInfo {
	char *command;
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
	void Register(char *com, command_func_t* f){
		CommandInfo i;
		i.command = com;
		i.func    = f;
		cinfo.push_back(i);
	}
	int Exec(const char *script);
	int Exec(std::ifstream &script);

	int exec_command(std::string);
//private:
	void sh_proc(void);
private:
	Emulator *emu;
	std::thread *sh_thread;
	std::vector<CommandInfo> cinfo;
	std::ifstream *ifs;
	std::ofstream *ofs;
	bool script_flg;
	static std::streambuf *cin_buf, *cout_buf;
	static int sh_num;
};

#endif
