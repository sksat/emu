ASM=$(shell ls *.asm)
BIN=$(ASM:.asm=.bin)
LOG=$(BIN:.bin=.log)

EMU=../emu

%.bin:%.asm
	nasm -o $@ $<

%.log:%.bin
	@./test.sh $(EMU) $< $@

.PHONY: test clean

test:
	rm -rf *.log
	make -C ..
	make $(BIN)
	make $(LOG)

clean:
	rm -f *.log *.bin
