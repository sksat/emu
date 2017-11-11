include ../../common.mk

LIBX86 := x86.a

X86OBJS = emulator.o register.o insndata.o instruction.o instruction16.o instruction32.o

$(LIBX86) : $(X86OBJS)
	ar rcs $@ $^

clean:
	rm -f $(LIBX86) $(X86OBJS)
