TARGET	= helloos.img
OBJS	= startup.o main.o

HARIB27F_APP = bball

HARIB_VER = harib00j
#HARIB_DAY = 02

default:
	make $(TARGET)

set_branch:
ifdef HARIB_VER
	$(eval HARIB_BRANCH=refs/tags/$(HARIB_VER))
else
ifdef HARIB_DAY
	$(eval HARIB_BRANCH=day/$(HARIB_DAY))
endif
endif

HARIB_BRANCH ?= master

CFLAGS  = -m32
CFLAGS  += -nostdlib -fno-asynchronous-unwind-tables -g -fno-stack-protector
LDFLAGS  = -melf_i386
LDFLAGS += --entry=start -Ttext=0x7c00

include ../common.mk

%.o:%.S
	gcc -m32 -c  $< -o $@

%.o:%.asm
	nasm -f elf $<

%.o:%.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f *.img *.bin *.o

tmp.elf:$(OBJS)
	ld $(LDFLAGS) $^ -o $@

test.bin:tmp.elf
	objcopy -O binary tmp.elf $@
	rm $<

z_tools:
	git clone https://github.com/HariboteOS/z_tools_linux.git
	mv z_tools_linux $@

haribote-os:
	git clone https://github.com/sk2sat/haribote-os.git
	cd $@ && ./install.sh

helloos.img:haribote-os
	cd $< && git checkout 2-4
	make -C $</src
	mv $</src/$@ ./$@

haribote.img: haribote-os set_branch DUMMY
	cd $< && git clean -f
	cd $< && git checkout $(HARIB_BRANCH) || \
		(echo -e "error:\n\t$(if $(HARIB_VER), version "\"$(HARIB_VER)\"", day "\"$(HARIB_DAY)\"") not found." && exit -1)
	make -C $</src
	mv $</src/$@ $@ || mv $</src/helloos.img $@

harib27f:z_tools
	git clone https://github.com/HariboteOS/harib27f.git

harib27f.img: harib27f
	cd $< && make -C apilib && make -C haribote
	cd $< && make -C $(HARIB27F_APP) haribote.img
	cp $</$(HARIB27F_APP)/haribote.img ./$@
