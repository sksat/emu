LIBARCH := arch.a

X86	:= x86/emulator.o x86/instruction.o x86/instruction16.o x86/instruction32.o x86/insndata.o
OSECPU	:= osecpu/emulator.o osecpu/instruction.o

all:$(COMMON)
	make $(X86)
	make $(OSECPU)
	ar rcs $(LIBARCH) */*.o

clean:
	rm */*.o
