BITS 32
extern main
global start

start:
	nop
	inc eax
	inc eax
	inc ecx
	inc edx
	inc esp
;	dec eax
	jmp start
