BITS 32
	ADD	AL, 0b10101010
	AND	AL, 0b01010101 ; AL=0
	OR	AL, 0b11111111 ; AL=255
	SUB	AL, 0b11111111 ; AL=0
	HLT