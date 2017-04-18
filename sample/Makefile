TARGET	= test.bin
OBJS	= startup.o

%.o:%.S
	gcc -m32 -c  $< -o $@

%.o:%.asm
	nasm -f elf $<

%.o:%.c
	gcc -m32 -nostdlib -fno-asynchronous-unwind-tables -g -fno-stack-protector -c $< -o $@

default:
	make $(TARGET)

clean:
	rm -f $(TARGET) *.o

$(TARGET):$(OBJS)
	ld -melf_i386 --entry=start --oformat=binary -Ttext 0x7c00 -o $@ $^

