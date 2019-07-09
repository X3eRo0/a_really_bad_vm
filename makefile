CFLAGS=-std=c++11

all: vm write

vm: vm.o vm2.o
	$(CXX) $(CFLAGS) vm.o vm2.o -o vm

vm2.o: vm2.cpp
	$(CXX) $(CFLAGS) -c vm2.cpp

vm.o: vm.h vm.cpp
	$(CXX) $(CFLAGS) -c vm.cpp

write:
	g++ write.cpp -o compile
clean:
	rm -f *.o vm compile program.bin
