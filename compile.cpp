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
    0x02040000,
    0x0c000020,
    0x02040012,
    0x02050000,
    0x02040019,
    0x0b000000,
  };
  while(program.size()!=511){
    program.push_back(0);
  }
  cout << program.size()<<endl;
  const char str[256][256] = {
        "Whats Your Name? \x09",
        "Hello \x09",
        "!\nnice to meet you\n\x09",
  };
  for (int j=0;j<256;j++){
    int k = 0;
    
    while(str[j][k]!='\x00'){
        
        program.push_back((char)str[j][k]);
        k++;
    }
  }


  /*
  cout<<"Loaded program"<<endl;
  for(i32 code : program) {
    cout<<hex<<code<<endl;
  }
  */

  for (i32 code : program)
  {
    ofile.write(reinterpret_cast<char *>(&code), sizeof(i32));
  }
  ofile.close();
}
