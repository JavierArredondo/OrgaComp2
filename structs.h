# include <stdio.h>
# ifndef _STRUCTS_H_
# define _STRUCTS_H_

typedef struct List
{
	int size;
	char** content;
} List;

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

typedef struct Program
{
	FILE* instFile;
	FILE* registerFile;
	FILE* traceFile;
	FILE* hazardFile;
} Program;

typedef struct Buffer
{
	int addPC;
	char inst[20];
	char rs[10];
	char rt[10];
	char rd[10];
	char labelBranch[64];
	int readData1;
	int readData2;
	int loadData;
	int signExtend;
	int ALUresult;
	int ADDresult;
	char MuxRegDst[10];
	int zero;
	UnitControl uc;
} Buffer;

typedef struct Processor
{
	int pointer;
	int* stack;
	int* memory;
	int PC;
	int* registersData;
	int* previewData;
	char** registersName;
	UnitControl uc;
	Buffer IF_ID;
	Buffer ID_EX;
	Buffer EX_MEM;
	Buffer MEM_WB;

	int cycle;
	int stallB;

	char* instr1;
	char* instr2;
	char* instr3;
	char* instr4;
	char* instr5;
	List* hazardControl;
	List* hazardData;
	List* nopList;
	List* IFlist;
	List* IDlist;
	List* EXlist;
	List* MEMlist;
	List* WBlist;
} Processor;

#endif