int abs(int i){
	if(i >= 0)
		return i;
	return -i;
}

void loop2(){
	volatile int i;
	for(i=0;i<10000;i++);
	return;
}

int main(void){
	loop2();
	return abs(3);
}
