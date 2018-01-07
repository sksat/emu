LIBOSECPU := osecpu.a
OSECPUOBJS:= emulator.o instruction.o

$(LIBOSECPU) : $(OSECPUOBJS)
	ar rcs $@ $^

include ../../common.mk

clean: _clean
