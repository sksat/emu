BITS 32
extern main
global start

start:
	nop
	nop
	mov dword [0x20], 5
;	mov dword [ebp+4], eax
	;	dec eax
	jmp start
