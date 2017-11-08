TARGET	:= emu
OBJS	:= main.o emulator_base.o emulator.o register_base.o memory.o
OBJS	+= device/device.a
OBJS	+= font/font.o
#OBJS	+= shell/shell.a
#OBJS	+= gui/gui.a
OBJS	+= arch/arch.a

include common.mk

LDFLAGS	+= -pthread -lglfw -lGL

EMU_BIN	:= sample/helloos.img
RUNFLAGS:= --arch x86 --junk-bios --memory-size 1 --fda $(EMU_BIN)

export

%.o:%.c
	$(CC) -c -o $@ $< $(CFLAGS)

%.o:%.cc
	$(CXX) -c -o $@ $< $(CXXFLAGS)

default:
	make $(TARGET)

run: $(TARGET) $(EMU_BIN)
	make
	./$(TARGET) $(RUNFLAGS)

clean :
	make -C gui clean
	#make -C shell clean
	make -C font clean
	make -C device clean
	make -C sample clean
	make -C arch clean
	rm -f $(TARGET) $(OBJS)

full :
	make clean
	make

full_run :
	make full
	make  run

$(TARGET) : $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(EMU_BIN):
	make -C sample

device/device.a:
	make -C device

font/font.o:
	make -C font

shell/shell.a:
	make -C shell

gui/gui.a:
	make -C gui

arch/arch.a:
	make -C arch
