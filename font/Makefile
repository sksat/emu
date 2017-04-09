DEPTH  := ../../../
include ../com_mak.txt

FONT_LIB := font.a
FONT_OBJS:= hankaku.o

%.o : %.obj
	objcopy -I pe-i386 $< -O elf32-i386 $@

%.obj : %.bin
	$(BIN2OBJ) $< $@ $*

%.bin : %.txt
	$(MAKEFONT) $< $@

default:
	make $(FONT_LIB)

clean:
	rm -f *.a *.o *.obj *.bin

$(FONT_LIB) : $(FONT_OBJS)
	ar rcs $@ $^
