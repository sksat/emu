BITS 32
extern main
global start
;	org 0x7c00

start:
;	call main
	mov dword [0x00], 10000000
	call loop
	jmp 0

loop:
	inc ecx
	cmp ecx,[0x00]
	jnz loop
	ret
