LIBOSECPU := osecpu.a
OSECPUOBJS:= emulator.o instruction.o

$(LIBOSECPU) : $(OSECPUOBJS)
	emar rcs $@ $^

include ../../common.mk

clean: _clean
