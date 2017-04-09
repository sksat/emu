#include <iostream>
#include <string>
#include <sstream>
#include "instruction.h"

using namespace std;

void Instruction::Init(){
	for(char c=0x00;c<=0xff;c++)
		insn_func[c] = nullptr;
}

void Instruction::Exec(Emulator *emu, uint8_t code){
	if(insn_func[code] == nullptr){
		stringstream ss;
		ss<<"not implemented: code = "<<hex<<showbase<<(int)code<<endl;
		throw ss.str();
	}
}


