TARGET	= helloos.img
OBJS	= startup.o main.o

HARIB27F_APP = bball
HARIB_VER    = harib00b

CFLAGS  = -m32
CFLAGS  += -nostdlib -fno-asynchronous-unwind-tables -g -fno-stack-protector
LDFLAGS  = -melf_i386
LDFLAGS += --entry=start -Ttext=0x7c00

%.o:%.S
	gcc -m32 -c  $< -o $@

%.o:%.asm
	nasm -f elf $<

%.o:%.c
	gcc $(CFLAGS) -c $< -o $@

default:
	make $(TARGET)

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

haribote.img: haribote-os
	cd $< && git checkout refs/tags/$(HARIB_VER)
	make -C $</src
	mv $</src/$@ $@

harib27f:z_tools
	git clone https://github.com/HariboteOS/harib27f.git

harib27f.img: harib27f
	cd $< && make -C apilib && make -C haribote
	cd $< && make -C $(HARIB27F_APP) haribote.img
	cp $</$(HARIB27F_APP)/haribote.img ./$@
