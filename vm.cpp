#include <iostream>
#include "vm.h"

using namespace std;

instructions decode(i32 instruction){

	struct instructions ins ; // getting a instructions object

	// decoding registers according to definition
	ins.opcode 		= ( instruction & 0xff000000 ) >> 24; 
	ins.reg_id1 	= ( instruction & 0x00ff0000 ) >> 16;
	ins.reg_id2     = ( instruction & 0x0000ff00 ) >>  8;
	ins.imm			= ( instruction & 0x0000ffff )		;

	return ins;
};



void VM::execute(i32 code){

	struct instructions ins = decode(code);
	i32 opcode 	= ins.opcode ;
	i32 reg_id1 = ins.reg_id1;
	i32 reg_id2 = ins.reg_id2;
	i32 imm 	= ins.imm	 ;

	//	DEBUG
	//	printf("instruction : 0x%x\n",code);
	
	if (reg_id2 > 3 and opcode == 0x09){ 
		
		//	we will use reg_id2 as a wayz to pass zf as an argument to an instruction
		//	only when reg_id is greater than 3


		//	DEBUG
		//	printf("reg_id2 = zero flag (%d)\n",zf);

		reg_id2 = 2;
		reg[reg_id2] = zf ;
	}

	//printf("[+] opcode : 0x%x\n[+] reg_id1 : 0x%x\n[+] reg_id2 : 0x%x\n[+] imm : 0x%x\n",opcode,reg_id1,reg_id2,imm);

	switch(opcode){

		case 0x01: //[MOV] mov the imm value in one of the grp represented by reg_id1
			reg[reg_id1] = imm;
			break;

		case 0x02: //[PRINT] print the value in the register specified by reg_id1
			switch(reg_id2){ // switch for format string
				case 0x01:   // print as int
					printf("%d\n",reg[reg_id2]);break;
				case 0x02:	 // print as char
					printf("%c\n",reg[reg_id2]);;break;
				default:
					cout<<"Invalid format"<<endl;
			};
			break;

		case 0x3:	//[PUSH] value on stack and increment esp
			memory[++esp] = imm;
			break;

		case 0x4:	//[POP] value in reg and decrement esp
			reg[reg_id1] = memory[esp--];
			break;

		case 0x05:	//[ADD] Store the sum of registers in the first register
			reg[reg_id1] = reg[reg_id1] + reg[reg_id2];
			break;

		case 0x06:	//[MUL] Store the product of registers in the first register
			reg[reg_id1] = reg[reg_id1] * reg[reg_id2];
			break;

		case 0x07:	//[SUB] Store the difference of registers in the first register
			reg[reg_id1] = reg[reg_id1] - reg[reg_id2];
			break;

		case 0x08:	//[DIV] Store the result of devision of registers in the first register
			reg[reg_id1] = reg[reg_id1] / reg[reg_id2];
			break;

		case 0x9:	//cmp a register with register or with a imm value
			switch(reg[reg_id2]){
				case 0x00:	// if reg[reg_id2] is 0 then compare register with imm value
							// otherwise compare register with register 
					
					//	DEBUG
					//	printf("comparing %d with %d\n",reg[reg_id1],imm);
					
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
					
					//	DEBUG
						printf("comparing %d with (reg) %d\n",reg[reg_id1],reg[reg_id2]);
					
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

		case 0x0a:	//[JMP] jump to specified line (line number is passed as imm value)
			switch(reg_id1){
					//	if reg[reg_id1] is 1 then do a je 	(jump is zf is set)
					//	if reg[reg_id1] is 2 then do a jne	(jump if zf is not set) 
					//	otherwise just jump to the given line
				case 0x01:
					if(zf){
						eip = imm;
					}
					break;
				case 0x02:
					if(!zf){
						eip = imm;
					}
					break;

				default:
					eip = imm;
			}
			break;

		case 0x0b://[HALT] will stop the execution of vm
			isrunning=0;
			break;

		default:  // Segmentation Fault
			cout<<"Invalid Instruction (SEGV) eip :";
			printf("0x%x code : 0x%x\n",eip,code);

	}

};

void VM::fetch(){
	eip++;
}
