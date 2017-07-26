BITS 32
extern main
global start
;	org 0x7c00
start:
	call main
	jmp 0
