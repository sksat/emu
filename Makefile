TARGET	:= emu
OBJS	:= main.o emulator.o register.o memory.o
OBJS	+= instruction.o instruction16.o instruction32.o
OBJS	+= device/device.a
OBJS	+= font/font.a
OBJS	+= shell/shell.a
OBJS	+= gui/gui.a

CC	:= gcc
CXX	:= g++

CFLAGS	:= -g
CXXFLAGS:= -std=c++11 -g
LDFLAGS	:= -pthread -lglfw -lGL

EMU_BIN	:= sample/test.bin
RUNFLAGS:= 

export

%.o:%.c
	$(CC) -c -o $@ $< $(CFLAGS)

%.o:%.cc
	$(CXX) -c -o $@ $< $(CXXFLAGS)

default:
	make -C gui
	make -C shell
	make -C font
	make -C device
	make -C sample
	make $(TARGET)

run: $(TARGET) $(EMU_BIN)
	make
	./$(TARGET) $(RUNFLAGS)

clean :
	make -C gui clean
	make -C shell clean
	make -C font clean
	make -C device clean
	make -C sample clean
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

font/font.a:
	make -C font

shell/shell.a:
	make -C shell

gui/gui.a:
	make -C gui

