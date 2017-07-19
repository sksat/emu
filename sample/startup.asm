BITS 32
extern main
global start

start:
	nop
	nop
	inc eax
	inc eax
	inc ecx
	inc edx
	inc esp
;	dec eax
	jmp start
