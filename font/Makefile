DEPTH  := ../../../
include ../com_mak.txt

MAKEFONT		:= makefont
MAKEFONT_OBJS	:= makefont.o
FONT_OBJ		:= font.o

%.bin : %.txt $(MAKEFONT)
	./$(MAKEFONT) $< $@

default:
	make $(FONT_OBJ)
	make hankaku.bin

clean:
	rm -f *.o *.bin $(MAKEFONT)

$(MAKEFONT): makefont.c
	gcc $^ -o $@
