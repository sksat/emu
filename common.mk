GIT_COMMIT_ID  := $(shell git log -1 --format='%H')
GIT_COMMIT_DATE:= $(shell git log -1 --format='%ad')

CC	:= gcc
CXX	:= g++

CFLAGS   = -g
CXXFLAGS = -std=c++14 -g -Wall
CXXFLAGS += -DGIT_COMMIT_ID="\"$(GIT_COMMIT_ID)\""
CXXFLAGS += -DGIT_COMMIT_DATE="\"$(GIT_COMMIT_DATE)\""

%.o:%.c
	$(CC) -c -o $@ $< $(CFLAGS)

%.o:%.cc
	$(CXX) -c -o $@ $< $(CXXFLAGS)
