LIBARCH := arch.a

X86	:= x86/emulator.o
OSECPU	:= osecpu/emulator.o

all:$(COMMON)
	make $(X86)
	make $(OSECPU)
	ar rcs $(LIBARCH) */*.o

clean:
	rm */*.o
