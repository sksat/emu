TARGET	= test.bin
OBJS	= startup.o main.o

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
	rm -f $(TARGET) *.o

tmp.elf:$(OBJS)
	ld $(LDFLAGS) $^ -o $@

$(TARGET):tmp.elf
	objcopy -O binary tmp.elf $@
	rm $<
