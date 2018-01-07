GIT_COMMIT_ID  := $(shell git log -1 --format='%H')
GIT_COMMIT_DATE:= $(shell git log -1 --format='%ad')

CC	:= gcc
CXX	:= g++

INCFLAGS = -I./libsksat
LDFLAGS  = 

CFLAGS   = $(INCFLAGS) -MD -Wall -g
CXXFLAGS = $(INCFLAGS) -MD -std=c++14 -Wall -g
CXXFLAGS += -DGIT_COMMIT_ID="\"$(GIT_COMMIT_ID)\"" -DGIT_COMMIT_DATE="\"$(GIT_COMMIT_DATE)\""
CXXFLAGS += -DDEBUG

%.o:%.c $(HEAD)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o:%.cc $(HEADXX)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

DUMMY:
.PHONE: DUMMY

_clean: DUMMY
	rm -f *.o *.a *.d

-include *.d
