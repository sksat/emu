#include "emulator.h"

Emulator::Emulator(){
	
}

void Emulator::Init(int memory_size){
	bit_mode = DEFAULT_BIT_MODE;
	memory.Init(memory_size);
}

int Emulator::GetBitMode(){
	return bit_mode;
}

bool Emulator::IsHalt(){
	return halt_flg;
}

uint8_t Emulator::GetCode8(int index){
	return memory[pc.reg64 + index];	// 一応64bitにしておく
}


