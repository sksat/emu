#include <iostream>
#include "_emulator.h"

using namespace std;

int main(int argc, char **argv){

try{
	Emulator *emu;
	emu = new Emulator(ARCH::x86);

	cout<<"emulation start"<<endl;

	while(true){
		
	}

}catch(const char *msg){
	cout<<msg<<endl;
}

}
