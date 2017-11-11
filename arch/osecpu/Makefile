include ../../common.mk

LIBOSECPU := osecpu.a
OSECPUOBJS:= emulator.o instruction.o

$(LIBOSECPU) : $(OSECPUOBJS)
	ar rcs $@ $^

clean:
	rm -f $(LIBOSECPU) $(OSECPUOBJS)
