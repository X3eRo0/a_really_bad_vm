#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <string.h>

#include "vm.h"

using namespace std;

vector<i32> load_bytecode(char *filename){

	ifstream r(filename, ios::binary);
	i32 i;
	vector<i32> prog;

	while (r.read((char*)&i, sizeof(i))) {
		prog.push_back(i);
	}
	return prog;
}

int main(int argc, char *argv[]){

	
	if (argc < 2){
		cout << "Usage: "<<argv[0]<<" <bytecode>"<<endl;
		exit(1);
	}
	


	vector<i32> program = load_bytecode(argv[1]);

	/*
	i32 program[11] = {

		0x01010004,
		0x01020005,
		0x09010200,
		0x01030001,
		0x0a010008,
		0x0101dead,
		0x02010100,
		0x0b000000,
		0x0101f00d,
		0x02010100,
		0x0b000000,

	};
	*/

	VM vm;
	while(vm.isrunning){
		vm.fetch();
		vm.execute(program[vm.eip-1]);
		/*
		vm.execute(0x01010004);
		vm.execute(0x01020005);
		vm.execute(0x05010200);
		vm.execute(0x02010100);
		vm.execute(0x0b000000);
		*/
	}
	return 0;

}

