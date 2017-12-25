include ../common.mk

LIBARCH  := arch.a
ARCH_LIBS:= x86/x86.a osecpu/osecpu.a

all: 
	make $(LIBARCH)

$(LIBARCH): $(ARCH_LIBS)
	rm -f $@
	ar cqT $@ $^
	echo -e "create $@\naddlib $@\nsave\nend" | ar -M

clean:
	make -C x86 clean
	make -C osecpu clean

x86/x86.a:
	make -C x86

osecpu/osecpu.a:
	make -C osecpu
