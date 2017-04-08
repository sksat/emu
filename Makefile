TARGET	:= emu
OBJS	:= main.o emulator.o register.o memory.o
OBJS	+= instruction.o instruction16.o instruction32.o

CC	:= gcc
CXX	:= g++

CFLAGS	:= -g
CXXFLAGS:= -std=c++11 -g
LDFLAGS	:= 

EMU_BIN	:= sample/test.bin
RUNFLAGS:= $(EMU_BIN)

%.o:%.c
	$(CC) -c -o $@ $< $(CFLAGS)

%.o:%.cc
	$(CXX) -c -o $@ $< $(CXXFLAGS)

default:
	make $(TARGET)

run: $(TARGET) $(EMU_BIN)
	./$(TARGET) $(RUNFLAGS)

clean :
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
