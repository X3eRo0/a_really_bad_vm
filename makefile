CFLAGS=-std=c++11
CC=g++

all: machine compile

machine: vm.cpp main.cpp
	$(CC) $(CFLAGS) $^ -o $@

compile: compile.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f machine compile program.bin
