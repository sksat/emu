# default build type
BUILD_TYPE = Debug

TARGET	= emu
OBJS	= main.o emulator_base.o emulator.o register_base.o memory.o \
		  device/device.a \
		  font/font.o \
		  gui.o \
		  arch/x86/x86.a \
		  debug.o

LDFLAGS	+= -pthread -lglfw -lGL

EMU_BIN	:= haribote.img
ARCH	:= x86
MEMSIZE	:= 4
BIOS	:= --junk-bios
FLOPPY	:= --fda sample/$(EMU_BIN)
DEV	:= $(FLOPPY)
UI	:= --gui
RUNFLAGS:= --arch $(ARCH) --memory-size $(MEMSIZE) $(BIOS) $(DEV) $(UI)

SRCS=main.cc emulator.cc emulator_base.cc register_base.cc memory.cc \
	 gui.cc debug.cc \
	 device/floppy.cc device/display.cc \
	 arch/osecpu/emulator.cc arch/osecpu/instruction.cc \
	 arch/x86/emulator.cc arch/x86/register.cc arch/x86/insndata.cc arch/x86/instruction.cc arch/x86/instruction16.cc arch/x86/instruction32.cc

CXXFLAGS+=-DNO_DEBUG
CXXFLAGS+=-s DISABLE_EXCEPTION_CATCHING=1
CXXFLAGS+=-s USE_PTHREADS=1 -s PTHREAD_POOL_SIZE=2
#CXXFLAGS+=-s LEGACY_GL_EMULATION=1 -s USE_GLFW=3
CXXFLAGS+=-s ASYNCIFY
CXXFLAGS+=--shell-file _emu.html

default:
	#make -C arch
	#make $(TARGET)
	make -C sample haribote.img
	emcc $(CXXFLAGS) $(SRCS) -s WASM=1 -o emu.html --preload-file sample/haribote.img --preload-file font/hankaku.bin

include common.mk
export

#CFLAGS   += --target=wasm32 -emit-llvm -S
#CXXFLAGS += --target=wasm32 -emit-llvm -S

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
