#include <stdio.h>
#include <stdlib.h>

#include "command.h"

using namespace std;

namespace shell {

void InitDefaultCommand(Shell *sh){
	sh->Register("exit", exit);
	sh->Register("test", test);
	sh->Register("print", print);
	sh->Register("init", init);
	sh->Register("load", load);
	sh->Register("run", run);
	sh->Register("startg", startg);
}

int exit(Shell *sh, vector<string> args){
	int i=0, size = args.size();
	if(size > 2){
		cout<<"exit: Too many arguments."<<endl;
		return -1;
	}

	if(size == 1) goto exit;
	sscanf(args[1].c_str(), "%d", &i);
exit:
	::exit(i);
}

int test(Shell *sh, vector<string> args){
	cout<<"test command."<<endl;
}

int print(Shell *sh, vector<string> args){
	Emulator *emu = sh->get_emu();
	if(emu == nullptr) throw "emulator is null";
	cout<<"EIP : "<<hex<<showbase<<emu->EIP<<endl;
	cout<<"ESP : "<<hex<<emu->ESP<<endl;
}

// init memory_size init_eip init_esp
int init(Shell *sh, vector<string> args){
try{
	Emulator *emu = sh->get_emu();
	int siz = args.size();
	int memory_size;
	uint32_t init_eip, init_esp;

	int tmp = args[1].size(), unit=1, unit_long=0;

	if(siz < 2){
		emu->Init(1 * MB, 0x7c00, 0x7c00);
		return 0;
	}
//	if(siz<4){ cout<<"init: Too few arguments."<<endl; goto init_usage;}
	if(siz>4){ cout<<"init: Too many arguments."<<endl; goto init_usage;}

	// get memory size
	if(args[1][tmp-1] == 'B' || args[1][tmp-1] == 'b'){
		cout<<"byte mode"<<endl;
		if(tmp == 2){
			unit_long = 1;
		}else{
			switch(args[1][tmp-2]){
			case 'k':
			case 'K':
				unit_long = 2;
				unit = KB;
				break;
			case 'm':
			case 'M':
				unit_long = 2;
				unit = MB;
				break;
			case 'g':
			case 'G':
				unit_long = 2;
				unit = GB;
				break;
			default:
				break;
			}
		}
		cout<<"unit = "<<unit<<endl;
	}

	memory_size = stoi(args[1].substr(0, tmp-unit_long));
//	cout<<"mem siz "<<memory_size<<endl;

	memory_size *= unit;

	// get eip and esp
	if(siz > 2)
		init_eip = stoi(args[2], nullptr, 0);
	if(siz > 3)
		init_esp = stoi(args[3], nullptr, 0);

	cout<<"memory size : "<<memory_size<<endl;
	cout<<"init eip : "<<hex<<showbase<<init_eip<<endl;
	cout<<"init esp : "<<init_esp<<endl;
	emu->Init(memory_size, init_eip, init_esp);
	return 0;
init_usage:
	cout<<"> init memory_size init_eip init_esp"<<endl;
	return -1;

}catch(invalid_argument i){
	cout<<"error: invalid size"<<endl;
	return -1;
}catch(out_of_range o){
	cout<<"error: out of range"<<endl;
	return -1;
}

}

int load(Shell *sh, vector<string> args){
	Emulator *emu = sh->get_emu();
	if(args.size() != 3) throw "option error.";
	if(args[1] == "binary") cout<<"loading binary..."<<endl;
	cout<<"loading \""<<args[2]<<"\"."<<endl;
	if(emu == nullptr) throw "emulator is null";
	cout<<"setting default addr: 0x7c00~"<<endl;
	emu->LoadBinary(args[2].c_str(), 0x7c00, 512);
}

int run(Shell *sh, vector<string> args){
	Emulator *emu = sh->get_emu();
	cout<<"runing..."<<endl;
	emu->Start();
	return 0;
}

//starting GUI
int startg(Shell *sh, vector<string> args){
	cout<<"starting GUI...";

	Gui *gui = new Gui();
//	gui->gui_proc();
	sh->set_gui(gui);
	gui->Start();

	cout<<endl;
}

};
