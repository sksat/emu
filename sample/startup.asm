BITS 32
extern main
global start

start:
	nop
	nop
	inc eax
	mov dword [0x20], 5
	add dword [0x20], eax
	jmp start
