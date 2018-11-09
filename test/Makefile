ASM_TESTS=loop

SRC=$(ASM_TESTS:=.asm)
BIN=$(ASM_TESTS:=.bin)

%.bin:%.asm
	nasm -o $@ $<

default:
	make $(BIN)
