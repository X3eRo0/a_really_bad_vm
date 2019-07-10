#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])
{

  ofstream ofile("program.bin", ios::binary);

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

  for (i32 code : program)
  {
    ofile.write(reinterpret_cast<char *>(&code), sizeof(i32));
  }
  ofile.close();
}
