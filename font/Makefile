DEPTH  := ../../../
include ../com_mak.txt

FONT_LIB := font.a
FONT_OBJS:= hankaku.o

BFD_TARGET:= elf64-x86-64

%.o : %.bin
	objcopy -I binary $< -O $(BFD_TARGET) -B i386 $@
	# objcopy -I elf32-x86-64 $@ --redefine-sym binary_hankaku=hankaku $@

%.bin : %.txt
	$(MAKEFONT) $< $@

default:
	make $(FONT_LIB)

clean:
	rm -f *.a *.o *.obj *.bin

$(FONT_LIB) : $(FONT_OBJS)
	ar rcs $@ $^
