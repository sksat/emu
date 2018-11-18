SRC=$(shell ls *.asm)
BIN=$(SRC:.asm=.bin)

%.bin:%.asm
	nasm -o $@ $<

default:
	make $(BIN)

clean:
	rm -f *.bin
