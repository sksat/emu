TARGET	:= device.a
OBJS	:= display.o floppy.o

default:
	make $(TARGET)

include ../common.mk

clean: _clean

$(TARGET):$(OBJS)
	ar rcs $@ $^
