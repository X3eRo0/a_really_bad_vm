CFLAGS=-std=c++11
CC=g++

all: vm compile

vm: vm.cpp main.cpp
	$(CC) $(CFLAGS) $^ -o $@

compile: compile.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f vm compile program.bin
