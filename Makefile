EMU = emu
OBJS= main.o emulator.o cpu.o insn.o

BIN=test/jmp.bin

CXXFLAGS= -std=c++1z -Wall
LDFLAGS = -lstdc++fs

%.o: %.cc $(shell ls *.h)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

.PHONY: default run test clean

default:
	make $(EMU)

run: $(EMU)
	./$(EMU) $(BIN)

test:
	make -C test
	./test.sh

clean:
	rm -f $(EMU)
	rm -f $(OBJS)

$(EMU): $(OBJS) $(shell ls *.h)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)
