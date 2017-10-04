# include <stdio.h>
#ifndef _STRUCTS_H_
#define _STRUCTS_H_

typedef struct Program
{
	FILE* instFile;          
	FILE* controlFile;       
	FILE* traceFile;         
	int pointer;             
	int* stack;              
	int* memory;             
	int PC;                  
	int* registersData;
	char** registersName;    
} Program;

typedef struct UnitControl
{
	char RegDst;
	char Jump;
	char Branch;
	char MemRead;
	char MemToReg;
	char* ALUOp;
	char MemWrite;
	char ALUSrc;
	char RegWrite;
} UnitControl;

typedef struct Instruction
{
	char type;
	char* name;
	char* rs;
	char* rt;
	char* rd;
	char* label;
	int inm;
} Instruction;

typedef struct List
{
	int size;
	Instruction* content;
} List;

#endif