BITS 32
extern main
global start

start:
	nop
	nop
	mov dword [eax], 5
	;	dec eax
	jmp start
