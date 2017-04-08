TARGET	:= device.a
OBJS	:= device.o display.o

CC	:= gcc
CXX	:= g++

CFLAGS	:= -g
CXXFLAGS:= -std=c++11 -g

%.o:%.c
	$(CC) -c $< $(CFLAGS)
%.o:%.cc
	$(CXX) -c $< $(CXXFLAGS)

default:
	make $(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)

$(TARGET):$(OBJS)
	ar rcs $@ $^
