# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include "structs.h"

/* Función que verifica si un archivo existe en el directorio. 
   name: Nombre del arrchivo (con extensión) que se quiere buscar en el directorio. 
   return: Entero que representa si existe el archivo o no, 0 (falso) o 1 (verdadero). */
FILE* inDirectory(char* name)
{
	FILE* myFile = NULL;
	myFile = fopen(name, "r");
	if(!myFile)
		return NULL;
	return myFile;
}
/* Función que inicializa los registros de MIPS
   return: Puntero a puntero a char que contiene el nombre de los registros. 
*/
char** initRegisterName()
{
	char* registers[] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};
	int i;
	char** myRegistersName = (char**)malloc(sizeof(char*) * 32);
	for(i = 0; i < 32; i++)
	{
		char* name;
		name = registers[i];
		myRegistersName[i] = name;
	}
	return myRegistersName;
}
/* Función que inicializa el valor de los registros en 0.
   return: Puntero a int que significa el valor de los registros, 
*/
int* initRegisterData()
{
	int* registers = (int*)malloc(sizeof(int) * 32); // Pude haber hecho un simple calloc.
	int i;
	for(i = 0; i < 32; i++)
		registers[i] = 0;
	return registers;
}
/* Función que se encarga de inicializar una linea de control.
   argumentos: Chars que indican el estado del bit de las líneas de control.
   return: Lineas de control con valor. 
*/
UnitControl initUnitControl(char a1, char a2, char a3, char a4, char a5, char* a6, char a7, char a8, char a9)
{
	UnitControl myUC;
	myUC.RegDst = a1;;
	myUC.Jump = a2;
	myUC.Branch = a3;
	myUC.MemRead = a4;
	myUC.MemToReg = a5;
	myUC.ALUOp = a6;
	myUC.MemWrite = a7;
	myUC.ALUSrc = a8;
	myUC.RegWrite = a9;
	return myUC;
}
/* Función que se encarga de realizar un NOP.
   return: Unidad de Control con valores ceros
*/
UnitControl nop()
{
	return initUnitControl('0', '0', '0', '0', '0', "00", '0', '0', '0');
}
/* Función que crea una lista de instrucciones.
   return: Lista de instrucciones.
*/
List* createList()
{
	List* myList = (List*)malloc(sizeof(List));
	myList->size = 0;
	myList->content = NULL;
	return myList;
}
/* Procedimiento que agrega una instrucción a la lista de instrucciones.
   myList: Lista a la cual se agregará una instruccion.
   myInstr: Instrucción que se agregará.
*/
void append(List* myList, Instruction myInstr)
{
	if(myList->size == 0)
    {
        myList->content = (Instruction*)malloc(sizeof(Instruction));
        if(myList->content != NULL)
            myList->content[0] = myInstr;
    }
    else
    {
        myList->content = (Instruction*)realloc(myList->content, (myList->size + 1) * sizeof(Instruction));
        if(myList->content != NULL)
            myList->content[myList->size] = myInstr;
    }
    myList->size++;
}
Instruction initInstruction()
{
	Instruction myInstr;
	myInstr.name = (char*)malloc(sizeof(char)*5);
	myInstr.rs = (char*)malloc(sizeof(char)*5);
	myInstr.rt = (char*)malloc(sizeof(char)*5);
	myInstr.rd = (char*)malloc(sizeof(char)*5);
	myInstr.label = (char*)malloc(sizeof(char)*64);
	return myInstr;
}
Instruction createInstructionR(char* name, FILE* myFile)
{
	char rs[10];
	char rt[10];
	char rd[10];
	fscanf(myFile, " %[^','], %[^','], %s", rd, rs, rt);
	Instruction myIns = initInstruction();
	myIns.type = 'R';
	myIns.name = name;
	myIns.rs = rs;
	myIns.rt = rt;
	myIns.rd = rd;
	return myIns;
}
Instruction createInstructionJ(char* name, FILE* myFile)
{
	char label[64];
	fscanf(myFile, "%s", label);
	strcat(label, ":");
	Instruction myIns = initInstruction();
	myIns.type = 'J';
	myIns.label = label;
	return myIns;
}
Instruction createInstructionI(char* name, FILE* myFIle)
{
	Instruction myIns = initInstruction();
	myIns.type = 'I';
	if(strcmp(name,"lw"))
		printf("Es lw");
	else if(strcmp(name, "sw"))
		printf("Es sw\n");
	else
	{
		printf("Es inm\n");
	}
	return myIns;
}
/* Función que permite obtener el índice de un regsitro. 
   myRegister: Registro la cual se le quiere obtener su índice.
   myProgram: Programa al cual se le modificará un registro específico.
   return: Índice del registro presentado. */
int getIndex(char* myRegister, Program* myProgram)
{
	int i;
	for(i = 0; i < 32; i++)
		if(strcmp(myRegister, myProgram->registersName[i]) == 0)
			return i;
	return -1;
}
/* LECTOR DE CODIGO MIPS: Si encuenta una instrucción determinada la ejecuta, así progesivamente. */ 
List* readMips(Program* myProgram)
{
	List* myList = createList();
	char instruction[20];
	Instruction myInst;
	while(feof(myProgram->instFile) == 0)
	{
		fscanf(myProgram->instFile, "%s", instruction);
		if(strcmp(instruction, "add") == 0)
			myInst = createInstructionR("add", myProgram->instFile);
		else if(strcmp(instruction, "sub") == 0)
			myInst = createInstructionR("sub", myProgram->instFile);
		else if(strcmp(instruction, "mul") == 0)
			myInst = createInstructionR("mul", myProgram->instFile);
		else if(strcmp(instruction, "div") == 0)
			myInst = createInstructionR("div", myProgram->instFile);
		else if(strcmp(instruction, "addi") == 0)
			myInst = createInstructionR("addi", myProgram->instFile);
		else if(strcmp(instruction, "subi") == 0)
			myInst = createInstructionR("subi", myProgram->instFile);
		else if(strcmp(instruction, "lw") == 0)
			myInst = createInstructionR("lw", myProgram->instFile);
		else if(strcmp(instruction, "sw") == 0)
			myInst = createInstructionR("sw", myProgram->instFile);
		else if(strcmp(instruction, "beq") == 0)
			myInst = createInstructionR("beq", myProgram->instFile);
		else if(strcmp(instruction, "j") == 0)
			myInst = createInstructionR("j", myProgram->instFile);
		//printRegisters(myProgram);
		append(myList, myInst);
	}
	return myList;
}