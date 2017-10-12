# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include "structs.h"

/* Función que verifica si un archivo existe en el directorio. 
   name: Nombre del arrchivo (con extensión) que se quiere buscar en el directorio. 
   return: Puntero al archivo.. */
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
		myRegistersName[i] = registers[i];
	return myRegistersName;
}
/* Función que inicializa el valor de los registros en 0.
   return: Puntero a int que significa el valor de los registros, 
*/
int* initRegisterData(Program* myProgram)
{
	int* registers = (int*)calloc(32, sizeof(int));
	int i, aux;
	char strAux[10];
	for(i = 0; i < 32; i++)
		fscanf(myProgram->registerFile, "%s %i\n", strAux, &registers[i]);
	return registers;
}
Processor* initProcessor(Program* myProgram)
{
	Processor* myProcessor = (Processor*)malloc(sizeof(Processor));
	myProcessor->PC = 0;
	myProcessor->registersName = initRegisterName();
	myProcessor->registersData = initRegisterData(myProgram);
	myProcessor->previewData = (int*)calloc(32, sizeof(int));
	myProcessor->stack = (int*)calloc(1001, sizeof(int));
	myProcessor->pointer = 0;
	myProcessor->instr1 = (char*)calloc(64, sizeof(char));
	myProcessor->instr2 = (char*)calloc(64, sizeof(char));
	myProcessor->instr3 = (char*)calloc(64, sizeof(char));
	myProcessor->instr4 = (char*)calloc(64, sizeof(char));
	myProcessor->ID_IF = initBuffer();
	myProcessor->IF_EX = initBuffer();
	myProcessor->EX_MEM = initBuffer();
	myProcessor->MEM_WB = initBuffer();
	return myProcessor;
}
/* Función que inicializa un programa. Con algunos registros precargados inicializamos el programa.
   return: Estructura del programa inicializado. */
Program* initProgram(FILE* firstFile, FILE* secFile, FILE* thirdFile)
{
	Program* myProgram = (Program*)malloc(sizeof(Program));
	if(myProgram == NULL)
		return NULL;
	myProgram->instFile = firstFile;
	myProgram->registerFile = secFile;
	myProgram->traceFile = thirdFile;
	return myProgram;
}
Buffer initBuffer()
{
	Buffer myBuffer;
	strcpy(myBuffer.inst, "0");
	strcpy(myBuffer.rs, "0");
	strcpy(myBuffer.rt, "0");
	strcpy(myBuffer.rd, "0");
	myBuffer.addPC = 0;
	myBuffer.readData1 = 0;
	myBuffer.readData2 = 0;
	myBuffer.signExtend = 0;
	myBuffer.ALUresult = 0;
	myBuffer.ADDresult = 0;
	myBuffer.zero = 0;
	return myBuffer;
}
/* Función que permite obtener el índice de un regsitro. 
   myRegister: Registro la cual se le quiere obtener su índice.
   myProgram: Programa al cual se le modificará un registro específico.
   return: Índice del registro presentado. */
int getIndex(char* myRegister, Processor* myProcessor)
{
	int i;
	for(i = 0; i < 32; i++)
		if(strcmp(myRegister, myProcessor->registersName[i]) == 0)
			return i;
	return -1;
}
/* Función que realiza el desplazamiento del program counter, va a una label en específico. 
   myProgram: 
   return: .*/
void goToLabel(Program* myProgram, Processor* myProcessor, char* myLabel)
{
	rewind(myProgram->instFile);
	myProcessor->PC = 0;
	char labelAux[50];
	do
	{
		fscanf(myProgram->instFile, "%s ", labelAux);
		if(strcmp(labelAux, "add") == 0 || strcmp(labelAux, "sub") == 0 || strcmp(labelAux, "beq") == 0 || strcmp(labelAux, "mul") == 0||
		   strcmp(labelAux, "sw") == 0  || strcmp(labelAux, "lw") == 0  || strcmp(labelAux, "j") == 0 || strcmp(labelAux, "addi") == 0 ||
		   strcmp(labelAux, "subi") == 0 || strcmp(labelAux, "div") == 0)
			myProcessor->PC++;
	}
	while(strcmp(myLabel, labelAux) != 0);
}
/* Procedimiento que nuestra por pantalla los registros y sus valores. o es trascendental en el funcionamiento del programa.
   myProgram:  Programa en cuesión.
   return: - */
void printRegisters(Processor* myProcessor)
{
	int i;
	for(i = 0; i < 32; i++)
		printf("%s: %d\n", myProcessor->registersName[i], myProcessor->registersData[i]);
	printf("\n");
}
// FUNCIONES QUE IMPLEMENTA MIPS
/* 1.- FORMATO R */
/* Función que realiza la adición de dos registros. 
   myProgram: Programa al cual se le ejecutará una adición.
   return: .*/
void add(Program* myProgram, Processor* myProcessor)
{
	char rs[10];
	char rt[10];
	char rd[10];
	fscanf(myProgram->instFile, " %[^','], %[^','], %s", rd, rs, rt);
	int indexRS = getIndex(rs, myProcessor);
	int indexRT = getIndex(rt, myProcessor);
	int indexRD = getIndex(rd, myProcessor);
	myProcessor->registersData[indexRD] = myProcessor->registersData[indexRS] + myProcessor->registersData[indexRT];
	myProcessor->PC++;
}
/* Función que realiza la sustracción de dos registros. 
   myProgram: Programa al cual se le ejecutará una sustracción.
   return: . */
void sub(Program* myProgram, Processor* myProcessor)
{
	char rs[10];
	char rt[10];
	char rd[10];
	fscanf(myProgram->instFile, " %[^','], %[^','], %s", rd, rs, rt);
	int indexRS = getIndex(rs, myProcessor); // Para obtener los datos de aquellos registros.
	int indexRT = getIndex(rt, myProcessor);
	int indexRD = getIndex(rd, myProcessor);
	myProcessor->registersData[indexRD] = myProcessor->registersData[indexRS] - myProcessor->registersData[indexRT]; // Operación realizada.
	myProcessor->PC++;
}
/* Función que realiza la multiplicación de dos registros. 
   myProgram: Programa al cual se le ejecutará una sustracción.
   return: . */
void mul(Program* myProgram, Processor* myProcessor)
{
	char rs[10];
	char rt[10];
	char rd[10];
	fscanf(myProgram->instFile, " %[^','], %[^','], %s", rd, rs, rt);
	int indexRS = getIndex(rs, myProcessor);
	int indexRT = getIndex(rt, myProcessor);
	int indexRD = getIndex(rd, myProcessor);
	myProcessor->registersData[indexRD] = myProcessor->registersData[indexRS] * myProcessor->registersData[indexRT];
	myProcessor->PC++;
}
/* Función que realiza la división de dos registros. 
   myProgram: Programa al cual se le ejecutará una sustracción.
   return: . */
void divi(Program* myProgram, Processor* myProcessor)
{
	char rs[10];
	char rt[10];
	char rd[10];
	fscanf(myProgram->instFile, " %[^','], %[^','], %s", rd, rs, rt);
	int indexRS = getIndex(rs, myProcessor);
	int indexRT = getIndex(rt, myProcessor);
	int indexRD = getIndex(rd, myProcessor);
	myProcessor->registersData[indexRD] = myProcessor->registersData[indexRS] / myProcessor->registersData[indexRT];
	myProcessor->PC++;
}
/* 2.- FORMATO I */
/* Función que realiza la suma inmediata de un registro con un número. 
   myProgram: Programa el cual ejecuta la operación.
   return: . */
void addi(Program* myProgram, Processor* myProcessor)
{
	char rs[10];
	char rt[10];
	int inm;
	fscanf(myProgram->instFile, " %[^','], %[^','], %i", rt, rs, &inm);
	int indexRS = getIndex(rs, myProcessor);
	int indexRT = getIndex(rt, myProcessor);
	myProcessor->registersData[indexRT] = myProcessor->registersData[indexRS] + inm;
	myProcessor->PC++;
}
/* Función que realiza la suma inmediata de un registro con un número. 
   myProgram: Programa el cual ejecuta la operación.
   return: . */
void subi(Program* myProgram, Processor* myProcessor)
{
	char rs[10];
	char rt[10];
	int inm;
	fscanf(myProgram->instFile, " %[^','], %[^','], %i", rt, rs, &inm);
	int indexRS = getIndex(rs, myProcessor);
	int indexRT = getIndex(rt, myProcessor);
	myProcessor->registersData[indexRT] = myProcessor->registersData[indexRS] - inm;
	myProcessor->PC++;
}
/* Función que almacena .
   myProgram: Programa que efectúa la operación.
   return: . */
void sw(Program* myProgram, Processor* myProcessor)
{
	char rs[10];
	char rt[10];
	int inm;
	fscanf(myProgram->instFile, " %[^','], %d(%[^')']", rt, &inm, rs);
	fscanf(myProgram->instFile, ")");
	if(strcmp(rs, "$sp") == 0) // Si se está en el stack pointer se realiza la siguiente operación.
	{
		myProcessor->pointer = myProcessor->registersData[29]/4;
		int pointerAux = myProcessor->pointer + (inm/4) + 1000;
		myProcessor->stack[pointerAux] = myProcessor->registersData[getIndex(rt, myProcessor)]; 
	}
	myProcessor->PC++;
}
/* Función que carga a un registro una palabra del stack pointer.
   myProgram: Progrma que efectúa la evalución.
   return: Booleano. */
void lw(Program* myProgram, Processor* myProcessor)
{
	char rs[10];
	char rt[10];
	int inm;
	fscanf(myProgram->instFile, " %[^','], %d(%[^')']", rt, &inm, rs);
	fscanf(myProgram->instFile, ")");
	if(strcmp(rs, "$sp") == 0)
	{
		myProcessor->pointer = myProcessor->registersData[29]/4;
		int pointerAux = myProcessor->pointer + (inm/4) + 1000;
		myProcessor->registersData[getIndex(rt, myProcessor)] = myProcessor->stack[pointerAux];
	}
	myProcessor->PC++;
}
/* Función que realiza la tarea comparativa. (==)
	myProgram: Programa que efectúa la comparación de dos registros.
	return: . */
void beq(Program* myProgram, Processor* myProcessor)
{
	char rs[10];
	char rd[10];
	char label[64];
	fscanf(myProgram->instFile, " %[^','], %[^','], %s", rs, rd, label);
	int indexRS = getIndex(rs, myProcessor);
	int indexRD = getIndex(rd, myProcessor);
	if(myProcessor->registersData[getIndex(rs, myProcessor)] == myProcessor->registersData[getIndex(rd, myProcessor)])
	{
		strcat(label, ":");
		goToLabel(myProgram, myProcessor, label);
	}
	myProcessor->PC++;
}
/* 3.- FORMATO J */
/* Función que realiza el "salto" de MIPS.
   myProgram: Programa que realiza la operación.
   return: . */
void j(Program* myProgram, Processor* myProcessor)
{
	char label[64];
	fscanf(myProgram->instFile, "%s", label);
	strcat(label, ":");
	goToLabel(myProgram, myProcessor,label);
}
void changeUnitControl(char a1, char a2, char a3, char a4, char a5, char* a6, char a7, char a8, char a9, Processor* myProcessor)
{
	myProcessor->uc.RegDst = a1;;
	myProcessor->uc.Jump = a2;
	myProcessor->uc.Branch = a3;
	myProcessor->uc.MemRead = a4;
	myProcessor->uc.MemToReg = a5;
	myProcessor->uc.ALUOp = a6;
	myProcessor->uc.MemWrite = a7;
	myProcessor->uc.ALUSrc = a8;
	myProcessor->uc.RegWrite = a9;
}
void setUnitControl(char* instr, Processor* myProcessor)
{
	UnitControl uc = myProcessor->uc;
	if(strcmp(instr, "add") == 0 || strcmp(instr, "sub") == 0 || strcmp(instr, "mul") == 0 || strcmp(instr, "div"))
		changeUnitControl('0', '0', '0', '0', '0', "10", '0', '0', '1', myProcessor);
	else if(strcmp(instr, "lw"))
		changeUnitControl('1', '0', '0', '1', '1', "00", '0', '1', '1', myProcessor);
	else if(strcmp(instr, "sw"))
		changeUnitControl(uc.RegDst, '0', '0', '0', uc.MemToReg, "00", '1', '1', uc.RegWrite, myProcessor);
	else if(strcmp(instr, "beq"))
		changeUnitControl(uc.RegDst, '0', '1', uc.MemRead, uc.MemToReg, "01", '0', '0', '0', myProcessor);
	else if(strcmp(instr, "addi"))
		changeUnitControl('1', '0', '0', '0', '0', "00", '0', '1', '1', myProcessor);
	else if(strcmp(instr, "subi"))
		changeUnitControl('1', '0', '0', '0', '0', "01", '0', '1', '1', myProcessor);
	else if(strcmp(instr, "j"))
		changeUnitControl(uc.RegDst, '0', '0', '0', uc.MemToReg, uc.ALUOp, '0', uc.ALUSrc, '0', myProcessor);
	else
		changeUnitControl('0', '0', '0', '0', '0', "00", '0', '0', '0', myProcessor);
}
// Etapas del camino de datos:
char* IF(Program* myProgram, Processor* myProcessor)
{
	char instruction[64];
	char rs[5];
	char rt[5];
	char rd[5];
	char label[64];
	int inm;
	char* line = (char)malloc(sizeof(char) * 20);
	while(feof(myProgram->insFile) == 0)
	{
		fscanf(myProgram->insFile, "%s", instruction);
		if(strcmp(instruction, "add") == 0 || strcmp(instruction, "sub") == 0 || strcmp(instruction, "mul") == 0 || strcmp(instruction, "div") == 0)
		{
			fscanf(myProgram->instFile, " %[^','], %[^','], %s", rd, rs, rt);
			sprintf(line, "%s %s, %s, %s", instruction, rd, rt, rs);
		}
		else if(strcmp(instruction, "addi") == 0 || strcmp(instruction, "subi") == 0)
		{
			fscanf(myProgram->instFile, " %[^','], %[^','], %i", rd, rs, &inm);
			sprintf(line, "%s %s, %s, %i", instruction, rd, rt, inm);
		}
		else if(strcmp(instruction, "lw") == 0 || strcmp(instruction, "sw") == 0)
		{
			fscanf(myProgram->instFile, " %[^','], %d(%[^')']", rt, &inm, rs);
			fscanf(myProgram->instFile, ")");
			sprintf(line, "%s %s, %i(%s)", instruction, rt, inm, rs);
		}
		else if(strcmp(instruction, "j") == 0)
		{
			fscanf(myProgram->instFile, "%s", label);
			sprintf(line, "j %s", label);
		}
		myProgram->PC++;
		return line;
	}
	return NULL;
}

void ID(Program* myProgram, Processor* myProcessor)
{
	char instruction[64];
	char rs[5];
	char rt[5];
	char rd[5];
	char label[64];
	int inm;
	sscanf(myProcessor->instr1, "%s", instruction);
	if(strcmp(instruction, "add") == 0 || strcmp(instruction, "sub") == 0 || strcmp(instruction, "mul") == 0 || strcmp(instruction, "div") == 0)
	{
		setUnitControl(instruction, myProcessor);
		sscanf(myProcessor->instr1, "%s %[^','], %[^','], %s", instruction, rd, rs, rt);
		myProcessor->IF_ID.readData1 = getIndex(rs, myProcessor);
		myProcessor->IF_ID.readData2 = getIndex(rt, myProcessor);
		myProcessor->IF_ID.inst = instruction;
		myProcessor->IF_ID.rs = rs;
		myProcessor->IF_ID.rt = rt;
		myProcessor->IF_ID.rd = rd;

	}
}

/* LECTOR DE CODIGO MIPS: Si encuenta una instrucción determinada la ejecuta, así progresivamente. */ 
void readMips(Program* myProgram, Processor* myProcessor)
{
	char instruction[20];
	while(feof(myProgram->instFile) == 0)
	{
		fscanf(myProgram->instFile, "%s", instruction);
		if(strcmp(instruction, "add") == 0)
			add(myProgram, myProcessor);
		else if(strcmp(instruction, "sub") == 0)
			sub(myProgram, myProcessor);
		else if(strcmp(instruction, "mul") == 0)
			mul(myProgram, myProcessor);
		else if(strcmp(instruction, "div") == 0)
			divi(myProgram, myProcessor);
		else if(strcmp(instruction, "addi") == 0)
			addi(myProgram, myProcessor);
		else if(strcmp(instruction, "subi") == 0)
			subi(myProgram, myProcessor);
		else if(strcmp(instruction, "lw") == 0)
			lw(myProgram, myProcessor);
		else if(strcmp(instruction, "sw") == 0)
			sw(myProgram, myProcessor);
		else if(strcmp(instruction, "beq") == 0)
			beq(myProgram, myProcessor);
		else if(strcmp(instruction, "j") == 0)
			j(myProgram, myProcessor);
	}
}