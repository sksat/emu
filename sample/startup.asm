BITS 32
extern main
global start

start:
	nop
	inc eax
	inc eax
	dec eax
	jmp start
