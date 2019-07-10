#ifndef VM_H
#define VM_H
#include <vector>

/*

instructions = {
	
	"mov":0x1, 
	"print":0x2,
	"push":0x3,
	"pop":0x4,
	"add":0x5,
	"mul":0x6,
	"sub":0x7,
	"div":0x8,
	"cmp":0x9,
	"jmp":0xa,
	"exit":0xb,
}

example program

mov eax,0x4 0x01010004
mov ecx,0x5 0x01020005
add eax,ecx 0x05010200
print eax c 0x02010200 (print eax as char)


mov eax,0x4
mov ebx,0x8
cmp eax,ebx
mov ecx,0001
cmp ecx, zf
jmp 9
mov eax,0xdead
print eax d
exit
mov eax,0xf00d
print eax d
exit

}

*/

typedef uint32_t i32; // i32 will be interpreted same as int32_t

class instructions
{
public:
	// declaring opcode and registers for decoding
	i32 opcode, reg_id1, reg_id2, eax, ebx, ecx, edx, imm;
};

// main vm

class VM
{
	i32 eip = 0x00;					//	instruction pointer
	i32 esp = -1;					//	stack pointer
	i32 memory[128];				//	stack
	std::vector<i32> program;		//	program
	i32 isrunning = 1;				//	is cpu running?

	i32 reg[4];						// array of gpr

	i32 zf = 0;						//	zero  flag
	i32 cf = 0;						//	carry flag
	
public:
	VM(char *programName);			//	constructor to load vm
	~VM();							//  destructor to cleanup everything
	void execute(i32);
	i32 fetch();
	void run();
};

#endif
