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
	0x02030079,
	0x0203006f,
	0x02030075,
	0x02030072,
	0x02030020,
	0x0203006e,
	0x02030061,
	0x0203006d,
	0x02030065,
	0x0203003a,
	0x02030020,
	0x0c00000a,
	0x04010000,
	0x02020100,
	0x04010000,
	0x02020100,
	0x04010000,
	0x02020100,
	0x04010000,
	0x02020100,
	0x04010000,
	0x02020100,
	0x04010000,
	0x02020100,
	0x04010000,
	0x02020100,
	0x04010000,
	0x02020100,
	0x04010000,
	0x02020100,
	0x04010000,
	0x02020100,
	0x0203000a,
	0x0b000000,

  };

  for (i32 code : program)
  {
    ofile.write(reinterpret_cast<char *>(&code), sizeof(i32));
  }
  ofile.close();
}
