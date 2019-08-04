#include <iostream>
#include <string.h>
#include <fstream>
#include "vm.h"

//#define DEBUG // enable this to enable debug output

using namespace std;

instructions decode(i32 instruction)
{

	instructions ins; // getting a instructions object

	// decoding registers according to definition
	ins.opcode      = (instruction & 0xff000000) >> 24;
	ins.reg_id1     = (instruction & 0x00ff0000) >> 16;
	ins.reg_id2     = (instruction & 0x0000ff00) >> 8;
	ins.imm 		= (instruction & 0x0000ffff);
        
        

	return ins;
};


VM::VM(char* filename){

       

	ifstream file(filename, ios::binary);
	i32 i;

	if(!file){
		printf("no such file or directory: %s\n",filename );
		exit(1);		
	}

	file.seekg(0,ios::end);
	int size = file.tellg();
	file.seekg(0);
	program.resize((size+3)/4);
	file.read((char*)&program[0],size);


#ifdef DEBUG
	cout<<size<<endl;
	cout<<"Loaded program"<<endl;
	for(i32 code : this->program) {
		cout<<hex<<code<<endl;
	}
#endif

	file.close();
}

void VM::execute(i32 code)
{

	instructions ins = decode(code);
	i32 opcode       = ins.opcode;
	i32 reg_id1      = ins.reg_id1;
	i32 reg_id2      = ins.reg_id2;
	i32 imm          = ins.imm;

	//	DEBUG
        #ifdef DEBUG
		printf("instruction : 0x%x\n",code);
        #endif
	
        if (reg_id2 == 4)
	{

		//	we will use reg_id2 as a wayz to pass zf as an argument to an instruction
		//	only when reg_id is greater than 3

		//	DEBUG
			printf("reg_id2 = zero flag (%d)\n",zf);

		reg_id2 = 2;
		reg[reg_id2] = zf;
	}

	//printf("[+] opcode : 0x%x\n[+] reg_id1 : 0x%x[ %d ]\n[+] reg_id2 : 0x%x[ %d ]\n[+] imm : 0x%x\n",opcode,reg_id1,reg[reg_id1],reg_id2,reg[reg_id2],imm);

	switch (opcode)
	{

	case 0x01: //[MOV] mov the imm value in one of the grp represented by reg_id1
          imm &= 0xff;
          switch(reg_id2){
            case 0x19:
              reg[reg_id1] = zf;
              break;
            default:
              reg[reg_id1] = imm;
              break;
          }break;

	case 0x02: //[PRINT] print the value in the register specified by reg_id1
		switch(reg_id1){ // switch for format string
			case 0x01:   // print as int
				printf("%d\n",reg[reg_id2]);break;
			case 0x02:	 // print as char
				printf("%c",reg[reg_id2]);break;
			case 0x03:
				printf("%c",imm );
				break;
                        case 0x04:
                            {
                                int i = 1;
                                char byte = (char)program[0x1ff+imm];
                                while(byte!='\x09'){
                                  printf("%c",byte);
                                  byte = (char)program[0x1ff+imm+i];
                                  i++;
                                }
                            }break;
                        case 0x05:
                            {
                                int i = 1;
                                char byte = (char)heap[strs[imm]];
                                while(byte!='\x09'){
                                  printf("%c",byte);
                                  byte = (char)heap[strs[imm]+i];
                                  i++;
                                }
                            }break;
			default:
				cout<<"Invalid format"<<endl;
		};
		break;

	case 0x3: //[PUSH] value on stack and increment esp
		memory[++esp] = imm;
		break;

	case 0x4: //[POP] value in reg and decrement esp
		reg[reg_id1] = memory[esp--];
		break;

	case 0x05: //[ADD] Store the sum of registers in the first register
                imm &= 0xff;
                switch(reg_id2){
                  case 0x5:
                    reg[reg_id1] = reg[reg_id1] + imm;
                    break;
                  default:
                    reg[reg_id1] += reg[reg_id2];
                    break;                 
                }
                break;


	case 0x06: //[MUL] Store the product of registers in the first register
		reg[reg_id1] = reg[reg_id1] * reg[reg_id2];
		break;

	case 0x07: //[SUB] Store the difference of registers in the first register
		reg[reg_id1] = reg[reg_id1] - reg[reg_id2];
		break;

	case 0x08: //[DIV] Store the result of devision of registers in the first register
		reg[reg_id1] = reg[reg_id1] / reg[reg_id2];
		break;

	case 0x9:	//cmp a register with register or with a imm value
		switch(reg_id2){

			case 0x05:	// if reg[reg_id2] is 0 then compare register with imm value
						// otherwise compare register with register 
				

				imm = imm & 0xff;
				#ifdef DEBUG	
				//	DEBUG
					printf("imm comparing 0x%x with 0x%x\n",reg[reg_id1],imm);
				#endif
				
				if(reg[reg_id1]==imm){

					zf = 1; cf = 0;
				}
				else if(reg[reg_id1] > imm){
					zf = 0; cf = 0;
				}
				else{
					zf = 0; cf = 1;
				}
				break;

                        case 0x06:
                                
                                imm = program[imm]&0xff;
				if(reg[reg_id1]==imm){

					zf = 1; cf = 0;
				}
				else if(reg[reg_id1] > imm){
					zf = 0; cf = 0;
				}
				else{
					zf = 0; cf = 1;
				}
				break;



			default:
				#ifdef DEBUG

				//	DEBUG
					printf("imm: %d\nreg[%d]:%d\n",imm,reg_id2,reg[reg_id2] );			
					printf("no imm comparing 0x%x with (reg) 0x%x\n",reg[reg_id1],reg[reg_id2]);
				#endif
				if(reg[reg_id1]==reg[reg_id2]){
					zf = 1; cf = 0;
				}
				else if(reg[reg_id1] > reg[reg_id1]){
					zf = 0; cf = 0;
				}
				else{
					zf = 0; cf = 1;
				}
				break;

		}
		break;

	case 0x0a: //[JMP] jump to specified line (line number is passed as imm value)
		switch (reg_id1)
		{
			//	if [reg_id1] is 1 then do a je 	(jump is zf is set)
			//	if [reg_id1] is 2 then do a jne	(jump if zf is not set)
			//	otherwise just jump to the given line
		case 0x01:
			if (zf)
			{
				eip = imm;
			}
			break;
		case 0x02:
			if (!zf)
			{
				eip = imm;
			}
			break;

		default:
			eip = imm;
		}
		break;

	case 0x0b: //[HALT] will stop the execution of vm
		isrunning = 0;
		break;
	
	case 0x0c: //[READ] read a char and place it into memory
                {
                        char buff[0x100];
                        fgets(buff,imm+1,stdin);
                        //printf("HTPR %d\n",hptr);
                        for(int i=0;i<0x100;i++){
                           if(buff[i] == '\x00' || buff[i] == '\x0a'){
                             buff[i]='\x09';
                             imm = i;
                             break;
                           }
                        }
                        //printf("[imm %d] [ %p ]\n",imm,heap);
                        int l=1;
                        memcpy(heap+hptr,buff,imm+1);
                        strs[strptr++]=hptr;
                        hptr+=imm+1;
                        #ifdef DEBUG
                        puts("heap dump");
                        for(int m=0;m<30;m++){
                          if(m%10==0){
                            printf("\n[ %02d ]",l);
                            l++;
                          }
                          printf("%c",heap[m]);
                        }
                        #endif
			break;
		}
        case 0x0d: // fetch the rom for a value and place that in a register
                imm &= 0xff;
                switch(imm){
                  case 0x1:
                    reg[reg_id1] = program[0x1ff+reg[reg_id2]];
                    break;
                  case 0x2:
                    reg[reg_id1] = heap[reg[reg_id2]];
                    break;
                }
                break;
        case 0x0e:{
                imm &= 0xff;
                switch(imm){
                  case 0x01:{
                              int address1 = strs[reg_id1];
                              int address2 = strs[reg_id2];
                              int i = 0,same = 1;
                              while(heap[address1+i]!='\x09'){
                                if(heap[address1+i] != '\x09' || heap[address2+i] != '\x09'){
                                  if(heap[address1+i]==heap[address2+i]){
                                    same = 1;
                                  }
                                  else{
                                    same = 0;
                                  }
                                }
                                else{
                                  same = 0;
                                }
                                i++;
                              }
                              zf = same;
                           }
                            
                  
                  case 0x02:{
                              int address1 = strs[reg_id1];
                              int address2 = reg_id2;
                              int i = 0,same = 1;
                              while(heap[address1+i]!='\x09'){
                                if(heap[address1+i] != '\x09' || program[address2+i] != '\x0a'){
                                  if(heap[address1+i]==program[0xff+address2+i]){
                                    same = 1;
                                  }
                                  else{
                                    same = 0;
                                  }
                                }
                                else{
                                  same = 0;
                                }
                                i++;
                              }
                              zf = same;
                           }
                            
                  }
              }break;
        case 0x0f:
                //printf("%d ^ %d",reg[reg_id1],imm);
                  if(reg[reg_id1]!='\x09')
                  reg[reg_id1] = reg[reg_id1] ^ imm;
                //printf(" : %d\n",reg[reg_id1]);

                  break;
	default: // Segmentation Fault
		cout << "Invalid Instruction (SEGV) eip :";
		printf("0x%x code : 0x%x\n", eip, code);
	}
}

i32 VM::fetch()
{
	return program[this->eip++];
}

void VM::run() {
	while(this->isrunning) {
		this->execute(this->fetch());
	}
}

VM::~VM() {

}
