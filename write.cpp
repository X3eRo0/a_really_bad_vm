#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[]){
  
  ofstream ofile("program.bin",ios::binary);

  typedef uint32_t i32;

  /*
   * mov ecx,0005
   * mov eax,0001
   * mov ebx,0001
   * mul eax,ecx
   * sub ecx,ebx
   * cmp ecx,0000
   * jne 3
   * print eax
   */
      
  vector<i32> program = {

    0x0afe0000,
    0x0b000000,

  };
  for (i32 i = 0; i < program.size(); i++) {
    ofile.write(reinterpret_cast<char *>(&program[i]), sizeof(i32));
  }
  ofile.close();  

}
