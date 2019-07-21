LIBARCH  := arch.a
ARCH_LIBS:= x86/x86.a osecpu/osecpu.a

all: 
	make $(LIBARCH)

include ../common.mk

$(LIBARCH): $(ARCH_LIBS)
	rm -f $@
	emar cqT $@ $^
	printf "create $@\naddlib $@\nsave\nend" | ar -M

clean: _clean
	make -C x86 clean
	make -C osecpu clean

x86/x86.a: DUMMY
	make -C x86

osecpu/osecpu.a: DUMMY
	make -C osecpu
