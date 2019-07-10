#include<iostream>
#include "vm.h"

using namespace std;

int main(int argc, char* argv[]) {
    if(argc!=2) {
        cerr<<"Usage: "<<argv[0]<<" <file to execute>"<<endl;
        return 1;
    }
    VM vm(argv[1]);
    vm.run();
}