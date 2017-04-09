#include <iostream>
#include <string>
#include <sstream>
#include "emulator.h"
#include "instruction.h"

using namespace std;

void Instruction::Init(){
	for(char c=0x00;c<=0xff;c++)
		insn_func[c] = nullptr;
	InitCommonInsn();
}

void Instruction::Exec(Emulator *emu, uint8_t code){
	if(insn_func[code] == nullptr){
		stringstream ss;
		ss<<"not implemented: code = "<<hex<<showbase<<(int)code<<endl;
		throw ss.str();
	}
}

namespace common_insn {

void nop(Emulator *emu){
	emu->pc.reg64++;
}



};

void Instruction::InitCommonInsn(void){
	using namespace common_insn;

	insn_func[0x90] = nop;
//	insn_func[0xFA] = cli;
//	insn_func[0xFB] = sti;
//	insn_func[0xF4] = hlt;
}


