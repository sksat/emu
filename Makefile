# default build type
BUILD_TYPE = Debug

TARGET	:= emu
OBJS	:= main.o emulator_base.o emulator.o register_base.o memory.o
OBJS	+= device/device.a
OBJS	+= font/font.o
#OBJS	+= shell/shell.a
#OBJS	+= gui/gui.a
OBJS	+= gui.o
OBJS	+= arch/arch.a
OBJS	+= debug.o

LDFLAGS	+= -pthread -lglfw -lGL

EMU_BIN	:= haribote.img
ARCH	:= x86
MEMSIZE	:= 4
BIOS	:= --junk-bios
FLOPPY	:= --fda sample/$(EMU_BIN)
DEV	:= $(FLOPPY)
UI	:= --gui
RUNFLAGS:= --arch $(ARCH) --memory-size $(MEMSIZE) $(BIOS) $(DEV) $(UI)

default:
	make $(TARGET)

include common.mk
export

run: $(TARGET) sample/$(EMU_BIN)
	make
	./$(TARGET) $(RUNFLAGS)

clean : _clean
	#make -C gui clean
	#make -C shell clean
	make -C font clean
	make -C device clean
	make -C sample clean
	make -C arch clean
	rm -f $(TARGET)

full :
	make clean
	make

full_run :
	make full
	make  run

$(TARGET) : $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

sample/$(EMU_BIN): DUMMY
	make -C sample $(EMU_BIN)

device/device.a: DUMMY
	make -C device

font/font.o: DUMMY
	make -C font

shell/shell.a: DUMMY
	make -C shell

gui/gui.a: DUMMY
	make -C gui

arch/arch.a: DUMMY
	make -C arch
