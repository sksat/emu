EMU = emu

OBJS	= main.o

%.o: %.cc
	$(CXX) -c -o $@ $< $(CXXFLAGS)

default:
	make $(EMU)

run: $(EMU)
	./$(EMU)

clean:
	rm -f $(EMU)
	rm -f $(OBJS)

$(EMU): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)
