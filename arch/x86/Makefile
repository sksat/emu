LIBX86 := x86.a

X86OBJS = emulator.o register.o insndata.o instruction.o instruction16.o instruction32.o

$(LIBX86) : $(X86OBJS)
	emar rcs $@ $^

include ../../common.mk

clean: _clean
