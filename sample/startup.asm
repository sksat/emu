BITS 32
;	org 0x7c00
start:
	sub esp, 16
	mov ebp, esp
	mov eax, 2
	mov dword [ebp+4],5
	add dword [ebp+4],eax
	mov esi, [ebp+4]
	inc dword [ebp+4]
	mov edi, [ebp+4]
	jmp 0
