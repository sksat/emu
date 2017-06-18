#include "emulator_base.h"

void EmulatorBase::Init(){
	finish_flg = false;
	InitInstructions();
	InitRegisters();
}
