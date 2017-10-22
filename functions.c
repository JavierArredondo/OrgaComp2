# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include "structs.h"
# include "functions.h"

# define ANSI_UNDERLINE_ON  "\x1b[4m"
# define ANSI_UNDERLINE_OFF "\x1b[24m"
# define ANSI_COLOR_RED     "\x1b[31m"
# define ANSI_COLOR_GREEN   "\x1b[32m"
# define ANSI_COLOR_YELLOW  "\x1b[33m"
# define ANSI_COLOR_BLUE    "\x1b[34m"
# define ANSI_COLOR_MAGENTA "\x1b[35m"
# define ANSI_COLOR_CYAN    "\x1b[36m"
# define ANSI_COLOR_RESET   "\x1b[0m"
# define ANSI_COLORBG_GREEN "\x1b[42m"
# define ANSI_COLORBG_CYAN "\x1b[46m"

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
/*	Función que inicializa la estructura UnitControl seteadas en cero.
	return: Estructura UnitControl.
*/
UnitControl initUnitControl()
{
	UnitControl uc;
	uc.RegDst = '0';
	uc.Jump = '0';
	uc.Branch = '0';
	uc.MemRead = '0';
	uc.MemToReg = '0';
	uc.ALUOp = (char*)malloc(sizeof(char)*2);
	strcpy(uc.ALUOp, "00");
	uc.MemWrite = '0';
	uc.ALUSrc = '0';
	uc.RegWrite = '0';
	return uc;
}
/*	Función que inicializa un Processor, con sus datos seteados en cero o convenciones que simulan nada.
	myProgram: Programa al cual se quiere leer los registros.
	return: puntero a Processor.
*/
Processor* initProcessor(Program* myProgram)
{
	Processor* myProcessor = (Processor*)malloc(sizeof(Processor));
	myProcessor->PC = 0;
	myProcessor->stallB = 0;
	myProcessor->cycle = 1;
	myProcessor->registersName = initRegisterName();
	myProcessor->registersData = initRegisterData(myProgram);
	myProcessor->previewData = (int*)calloc(32, sizeof(int));
	myProcessor->stack = (int*)calloc(1001, sizeof(int));
	myProcessor->pointer = 0;
	myProcessor->instr1 = (char*)calloc(64, sizeof(char));
	myProcessor->instr2 = (char*)calloc(64, sizeof(char));
	myProcessor->instr3 = (char*)calloc(64, sizeof(char));
	myProcessor->instr4 = (char*)calloc(64, sizeof(char));
	myProcessor->instr5 = (char*)calloc(64, sizeof(char));
	strcpy(myProcessor->instr1, "NONE");
	strcpy(myProcessor->instr2, "NONE");
	strcpy(myProcessor->instr3, "NONE");
	strcpy(myProcessor->instr4, "NONE");
	strcpy(myProcessor->instr5, "NONE");
	myProcessor->IF_ID = initBuffer();
	myProcessor->ID_EX = initBuffer();
	myProcessor->EX_MEM = initBuffer();
	myProcessor->MEM_WB = initBuffer();
	myProcessor->uc = initUnitControl();
	myProcessor->hazardControl = initList();
	myProcessor->hazardData = initList();
	myProcessor->IFlist = initList();
	myProcessor->IDlist = initList();
	myProcessor->EXlist = initList();
	myProcessor->MEMlist = initList();
	myProcessor->WBlist = initList();
	myProcessor->nopList = initList();
	return myProcessor;
}
/* Función que inicializa un programa. Con algunos registros precargados inicializamos el programa.
   return: Estructura del programa inicializado. */
Program* initProgram(FILE* firstFile, FILE* secFile, FILE* thirdFile, FILE* fourthFile)
{
	Program* myProgram = (Program*)malloc(sizeof(Program));
	if(myProgram == NULL)
		return NULL;
	myProgram->instFile = firstFile;
	myProgram->registerFile = secFile;
	myProgram->traceFile = thirdFile;
	myProgram->hazardFile = fourthFile;
	return myProgram;
}
/*	Función que inicializa los valores de un Buffer en cero, o conveciones que representan un valor nulo.
	return: estructura Buffer.
*/
Buffer initBuffer()
{
	Buffer myBuffer;
	strcpy(myBuffer.inst, "NONE");
	strcpy(myBuffer.rs, "0");
	strcpy(myBuffer.rt, "0");
	strcpy(myBuffer.rd, "0");
	strcpy(myBuffer.MuxRegDst, "0");
	strcpy(myBuffer.labelBranch, "0");
	myBuffer.addPC = 0;
	myBuffer.readData1 = 0;
	myBuffer.readData2 = 0;
	myBuffer.loadData = 0;
	myBuffer.signExtend = 0;
	myBuffer.ALUresult = 0;
	myBuffer.ADDresult = 0;
	myBuffer.zero = 0;
	myBuffer.uc = initUnitControl();
	return myBuffer;
}
/*	Función que inicializa una List. Esta funciona igual que una lista.
	return: Puntero a List
*/ 
List* initList()
{
	List* myList = (List*)malloc(sizeof(List));
	myList->size = 0;
	myList->content = NULL;
}
/*	Procedimiento que agrega un elemento a la lista List. Agrega el elemento al final de la lista. */
void append(List* myList, char* element)
{
	if(myList->size == 0)
	{
		myList->content = (char**)malloc(sizeof(char*));
		myList->content[0] = (char*)malloc(sizeof(char)*20);
		strcpy(myList->content[0], element); 
	}
	else
	{
		myList->content = (char**)realloc(myList->content, (myList->size + 1) * sizeof(char*));
		myList->content[myList->size] = (char*)malloc(sizeof(char)*20);
		strcpy(myList->content[myList->size], element);
	}
	myList->size++;
}
/*	Procedimiento que saca el útimo valor de la lista.
	return: Último elemento de la lista,
*/
char* pop(List* myList)
{
	int max = myList->size - 1;
	if(max < 0)
		return NULL;
	char* aux = (char*)malloc(sizeof(char)*30);
	strcpy(aux, myList->content[max]);
	myList->content = (char**)realloc(myList->content, max * sizeof(char*));
	return aux;

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
/* 	Función que dado un registro, obtiene el valor de dicho registro, por ejemplo puede calcular el valor que hay en 
	$t0.
	myRegister: registro a consultar.
	myProcessor: de donde saca el registro.
	return: valor que hay en el registro.*/
int getData(char* myRegister, Processor* myProcessor)
{
	return myProcessor->registersData[getIndex(myRegister, myProcessor)];
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
/*	Función que setea una estructura UnitControl con sus respectivas unidades de control.
	a's: Valores de las líneas de control.
	return: Unidad de control con lo valores deseados. 
*/
UnitControl changeUnitControl(char a1, char a2, char a3, char a4, char a5, char* a6, char a7, char a8, char a9)
{
	UnitControl myUnitControl;
	myUnitControl.RegDst = a1;;
	myUnitControl.Jump = a2;
	myUnitControl.Branch = a3;
	myUnitControl.MemRead = a4;
	myUnitControl.MemToReg = a5;
	myUnitControl.ALUOp = (char*)malloc(sizeof(char) * 2);
	strcpy(myUnitControl.ALUOp, a6);
	myUnitControl.MemWrite = a7;
	myUnitControl.ALUSrc = a8;
	myUnitControl.RegWrite = a9;
	return myUnitControl;
}
/*	Procedimiento que setea la unidad de control respecto a una intrucción.
	instr: Intrucción a setear.
	myProcessor: Procesador en el cual está la unidad de control. 
*/
void setUnitControl(char* instr, Processor* myProcessor)
{

	if(strcmp(instr, "add") == 0 || strcmp(instr, "sub") == 0 || strcmp(instr, "mul") == 0 || strcmp(instr, "div") == 0)
		myProcessor->uc = changeUnitControl('0', '0', '0', '0', '0', "10", '0', '0', '1');
	else if(strcmp(instr, "lw") == 0)
		myProcessor->uc = changeUnitControl('1', '0', '0', '1', '1', "00", '0', '1', '1');
	else if(strcmp(instr, "sw") == 0)
		myProcessor->uc = changeUnitControl(myProcessor->uc.RegDst, '0', '0', '0', myProcessor->uc.MemToReg, "00", '1', '1', myProcessor->uc.RegWrite);
	else if(strcmp(instr, "beq") == 0)
		myProcessor->uc = changeUnitControl(myProcessor->uc.RegDst, '0', '1', myProcessor->uc.MemRead, myProcessor->uc.MemToReg, "01", '0', '0', '0');
	else if(strcmp(instr, "addi") == 0)
		myProcessor->uc = changeUnitControl('1', '0', '0', '0', '0', "00", '0', '1', '1');
	else if(strcmp(instr, "subi") == 0)
		myProcessor->uc = changeUnitControl('1', '0', '0', '0', '0', "01", '0', '1', '1');
	else if(strcmp(instr, "j") == 0)
		myProcessor->uc = changeUnitControl(myProcessor->uc.RegDst, '0', '0', '0', myProcessor->uc.MemToReg, myProcessor->uc.ALUOp, '0', myProcessor->uc.ALUSrc, '0');
	else
		myProcessor->uc = changeUnitControl('0', '0', '0', '0', '0', "00", '0', '0', '0');
}
/* 	Función que realiza una copia de la unidad de control.
	u: Unidad de control a copiar.
	return: Nueva unidad de control con valores copiados de u.*/
UnitControl copyUnitControl(UnitControl u)
{
	UnitControl myUnitControl; 
	myUnitControl.RegDst = u.RegDst;;
	myUnitControl.Jump = u.Jump;
	myUnitControl.Branch = u.Branch;
	myUnitControl.MemRead = u.MemRead;
	myUnitControl.MemToReg = u.MemToReg;
	myUnitControl.ALUOp = (char*)malloc(sizeof(char)*2);
	strcpy(myUnitControl.ALUOp, u.ALUOp);
	myUnitControl.MemWrite = u.MemWrite;
	myUnitControl.ALUSrc = u.ALUSrc;
	myUnitControl.RegWrite = u.RegWrite;
	return myUnitControl;
}
/*	Procedimiento que copia un buffer.
	myBuffer: Buffer a copiar.
	b2: Buffer de destino.
*/
void copyBuffer(Buffer* myBuffer, Buffer b2)
{
	strcpy(myBuffer->inst, b2.inst);
	strcpy(myBuffer->rs, b2.rs);
	strcpy(myBuffer->rt, b2.rt);
	strcpy(myBuffer->rd, b2.rd);
	strcpy(myBuffer->MuxRegDst, b2.MuxRegDst);
	strcpy(myBuffer->labelBranch, b2.labelBranch);
	myBuffer->addPC = b2.addPC;
	myBuffer->readData1 = b2.readData1;
	myBuffer->readData2 = b2.readData2;
	myBuffer->loadData = b2.loadData;
	myBuffer->signExtend = b2.signExtend;
	myBuffer->ALUresult = b2.ALUresult;
	myBuffer->ADDresult = b2.ADDresult;
	myBuffer->zero = b2.zero;
}
// 	Etapas del camino de datos:
/* 	Para manejar cada etapa, se ha trabajado de forma conveniente para el desarrollador, se ha relacionado cada etapa con una instrucción:
	IF->  instr1.
	ID->  instr2.
	EX->  instr3.
	MEM-> instr4.
	WB->  instr5.
	En IF se lee una instrucción desde el archivo, en el resto de las etapas se procesa la instrucción correspondiente. Luego, se cambia de instrucción
	instr5->instr4
	instr4->instr3
	instr3->instr2
	instr2->instr1
	De esta manera se simula el Pipeline.

/*	Procedimiento que rescata la instrucción a procesar. Para esto se va leyendo el archivo instrucción a instrucción, en caso de termninar de leer el archivo
	se setea EOF a la intrucción 1.*/
void IF(Program* myProgram, Processor* myProcessor)
{
	char instruction[64];
	char rs[5];
	char rt[5];
	char rd[5];
	char label[64];
	int inm = 0;
	char* line = (char*)malloc(sizeof(char) * 64);
	strcpy(line, "NONE");
	strcpy(rs, "0");
	strcpy(rt, "0");
	strcpy(rd, "0");
	if(feof(myProgram->instFile) == 0)
	{
		fscanf(myProgram->instFile, "%s", instruction);
		if(strcmp(instruction, "add") == 0 || strcmp(instruction, "sub") == 0 || strcmp(instruction, "mul") == 0 || strcmp(instruction, "div") == 0)
		{
			fscanf(myProgram->instFile, " %[^','], %[^','], %s", rd, rs, rt);
			sprintf(line, "%s %s, %s, %s", instruction, rd, rs, rt);
		}
		else if(strcmp(instruction, "addi") == 0)
		{
			fscanf(myProgram->instFile, " %[^','], %[^','], %i", rt, rs, &inm);
			sprintf(line, "addi %s, %s, %i", rt, rs, inm);
		}
		else if(strcmp(instruction, "subi") == 0)
		{
			fscanf(myProgram->instFile, " %[^','], %[^','], %i", rt, rs, &inm);
			sprintf(line, "subi %s, %s, %i", rt, rs, inm);
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
		else if(strcmp(instruction, "sw") == 0)
		{
			fscanf(myProgram->instFile, " %[^','], %d(%[^')']", rt, &inm, rs);
			fscanf(myProgram->instFile, ")");
			sprintf(line, "%s %s, %i(%s)", instruction, rt, inm, rs);
		}
		else if(strcmp(instruction, "beq") == 0)
		{
			fscanf(myProgram->instFile, " %[^','], %[^','], %s", rs, rt, label);
			sprintf(line, "beq %s, %s, %s", rs, rt, label);
		}
		// SETEO DE IF_ID
		strcpy(myProcessor->IF_ID.rs, rs);
		strcpy(myProcessor->IF_ID.rt, rt);
		strcpy(myProcessor->IF_ID.rd, rd);
		myProcessor->IF_ID.signExtend = inm;
		myProcessor->IF_ID.addPC = myProcessor->PC;
		strcpy(myProcessor->IF_ID.inst, line);
		// SETEO DE PROCESADOR
		myProcessor->PC++;
		strcpy(myProcessor->instr1, line);
	}
	else
		strcpy(myProcessor->instr1, "EOF");
}
/*	Procedimiento que simlua etapa ID, setea la unidad de control con los valores correspondientes.*/
void ID(Program* myProgram, Processor* myProcessor)
{
	char instruction[64];
	char rs[5];
	char rt[5];
	char rd[5];
	char label[64];
	int inm;
	char* line = (char*)malloc(sizeof(char) * 64);

	sscanf(myProcessor->instr2, "%s", instruction);
	strcpy(myProcessor->ID_EX.inst, instruction);

	if(strcmp(instruction, "NONE") == 0 || strcmp(instruction, "NOP") == 0 || strcmp(instruction, "FLUSH") == 0)
		return;
	else if(strcmp(myProcessor->instr1, "EOF") == 0)
	{
		strcpy(myProcessor->ID_EX.inst, "EOF");
		return;
	}
	
	if(strcmp(instruction, "add") == 0 || strcmp(instruction, "sub") == 0 || strcmp(instruction, "mul") == 0 || strcmp(instruction, "div") == 0)
	{
		char aux[20];
		setUnitControl(instruction, myProcessor);
		copyBuffer(&myProcessor->ID_EX, myProcessor->IF_ID); //
		myProcessor->ID_EX.readData1 = getData(myProcessor->ID_EX.rs, myProcessor); //
		myProcessor->ID_EX.readData2 = getData(myProcessor->ID_EX.rt, myProcessor); //
		strcpy(myProcessor->ID_EX.labelBranch, "0");
		myProcessor->ID_EX.uc = copyUnitControl(myProcessor->uc);
	}
	else if(strcmp(instruction, "addi") == 0 || strcmp(instruction, "subi") == 0)
	{
		setUnitControl(instruction, myProcessor);
		copyBuffer(&myProcessor->ID_EX, myProcessor->IF_ID); //
		myProcessor->ID_EX.readData1 = getData(myProcessor->ID_EX.rs, myProcessor); //
		myProcessor->ID_EX.readData2 = 0; //
		strcpy(myProcessor->ID_EX.labelBranch, "0");
		myProcessor->ID_EX.uc = copyUnitControl(myProcessor->uc);
	}
	else if(strcmp(instruction, "lw") == 0)
	{
		setUnitControl(instruction, myProcessor);
		copyBuffer(&myProcessor->ID_EX, myProcessor->IF_ID); //
		myProcessor->ID_EX.readData1 = getData(myProcessor->ID_EX.rs, myProcessor); //
		myProcessor->ID_EX.readData2 = 0; //
		strcpy(myProcessor->ID_EX.labelBranch, "0");
		myProcessor->ID_EX.uc = copyUnitControl(myProcessor->uc);
	}
	else if(strcmp(instruction, "sw") == 0)
	{
		setUnitControl(instruction, myProcessor);
		copyBuffer(&myProcessor->ID_EX, myProcessor->IF_ID); //
		myProcessor->ID_EX.readData1 = getData(myProcessor->ID_EX.rs, myProcessor); //
		myProcessor->ID_EX.readData2 = getData(myProcessor->ID_EX.rt, myProcessor);; //
		strcpy(myProcessor->ID_EX.labelBranch, "0");
		myProcessor->ID_EX.uc = copyUnitControl(myProcessor->uc);
	}
	else if(strcmp(instruction, "beq") == 0)
	{
		setUnitControl("beq", myProcessor);
		sscanf(myProcessor->instr2, "%s %[^','], %[^','], %s", instruction, rs, rt, label);
		strcpy(myProcessor->ID_EX.labelBranch, label);
		strcpy(myProcessor->ID_EX.rd, "0");
		strcpy(myProcessor->ID_EX.rt, rt);
		strcpy(myProcessor->ID_EX.rs, rs);
		myProcessor->ID_EX.readData1 = myProcessor->registersData[getData(rs, myProcessor)];
		myProcessor->ID_EX.readData2 = myProcessor->registersData[getData(rt, myProcessor)];
	}
	else if(strcmp(instruction, "j") == 0)
	{
		setUnitControl("j", myProcessor);
		sscanf(myProcessor->instr2, "%s %s", instruction, label);
		strcpy(myProcessor->ID_EX.rd, "0");
		strcpy(myProcessor->ID_EX.rt, "0");
		strcpy(myProcessor->ID_EX.rs, "0");
		strcpy(myProcessor->ID_EX.labelBranch, "0");
		myProcessor->ID_EX.readData1 = 0;
		myProcessor->ID_EX.readData2 = 0;
		myProcessor->ID_EX.signExtend = 0;
		strcat(label, ":");
		goToLabel(myProgram, myProcessor, label);
		myProcessor->stallB = 1;
	}
}
/*	Procedimiento que simula etapa EX. Realiza la ejecución de la instrucción seteada. */
void EX(Program* myProgram, Processor* myProcessor)
{
	char instruction[64];
	char rs[5];
	char rt[5];
	char rd[5];
	int inm;

	sscanf(myProcessor->instr3, "%s", instruction);
	strcpy(myProcessor->EX_MEM.inst, myProcessor->instr3);

	if(strcmp(instruction, "NONE") == 0 || strcmp(instruction, "NOP") == 0 || strcmp(instruction, "FLUSH") == 0)
	{
		return;
	}
	else if(strcmp(instruction, "EOF") == 0)
	{
		strcpy(myProcessor->EX_MEM.inst, "EOF");
		return;
	}

	copyBuffer(&myProcessor->EX_MEM, myProcessor->ID_EX);

	if(strcmp(instruction, "add") == 0)
	{
		myProcessor->EX_MEM.ALUresult = myProcessor->ID_EX.readData1 + myProcessor->ID_EX.readData2;
		strcpy(myProcessor->EX_MEM.MuxRegDst, myProcessor->EX_MEM.rd);
	}
	else if(strcmp(instruction, "sub") == 0)
	{
		myProcessor->EX_MEM.ALUresult = myProcessor->ID_EX.readData1 - myProcessor->ID_EX.readData2;
		strcpy(myProcessor->EX_MEM.MuxRegDst, myProcessor->EX_MEM.rd);
	}
	else if(strcmp(instruction, "mul") == 0)
	{
		myProcessor->EX_MEM.ALUresult = myProcessor->ID_EX.readData1 * myProcessor->ID_EX.readData2;
		strcpy(myProcessor->EX_MEM.MuxRegDst, myProcessor->EX_MEM.rd);
	}
	else if(strcmp(instruction, "div") == 0)
	{
		myProcessor->EX_MEM.ALUresult = myProcessor->ID_EX.readData1 / myProcessor->ID_EX.readData2;
		strcpy(myProcessor->EX_MEM.MuxRegDst, myProcessor->EX_MEM.rd);
	}
	else if(strcmp(instruction, "addi") == 0)
	{
		myProcessor->EX_MEM.ALUresult = myProcessor->ID_EX.readData1 + myProcessor->ID_EX.signExtend;
		strcpy(myProcessor->EX_MEM.MuxRegDst, myProcessor->EX_MEM.rt);
	}
	else if(strcmp(instruction, "subi") == 0)
	{
		myProcessor->EX_MEM.ALUresult = myProcessor->ID_EX.readData1 - myProcessor->ID_EX.signExtend;
		strcpy(myProcessor->EX_MEM.MuxRegDst, myProcessor->EX_MEM.rt);
	}
	else if(strcmp(instruction, "lw") == 0)
	{
		myProcessor->EX_MEM.ALUresult = myProcessor->ID_EX.readData1 + myProcessor->ID_EX.signExtend;
		strcpy(myProcessor->EX_MEM.MuxRegDst, myProcessor->EX_MEM.rt);
	}
	else if(strcmp(instruction, "sw") == 0)
		myProcessor->EX_MEM.ALUresult = myProcessor->ID_EX.readData1 + myProcessor->ID_EX.signExtend;
	
	else if(strcmp(instruction, "beq") == 0)
	{
		char label[64];
		strcpy(label, myProcessor->ID_EX.labelBranch);
		myProcessor->EX_MEM.ALUresult = 0;
		strcpy(myProcessor->EX_MEM.MuxRegDst, "0");
		if (myProcessor->ID_EX.readData1 == myProcessor->ID_EX.readData2)
		{
			myProcessor->stallB = 2;
			myProcessor->EX_MEM.readData1 = 0;
			myProcessor->EX_MEM.readData2 = 0;
			myProcessor->EX_MEM.signExtend = 0;
			myProcessor->EX_MEM.zero = 1;
			strcpy(myProcessor->EX_MEM.rd, "0");
			strcpy(myProcessor->EX_MEM.rd, "0");
			strcat(label, ":");
			goToLabel(myProgram, myProcessor, label);
		}
	}
	myProcessor->EX_MEM.uc = copyUnitControl(myProcessor->ID_EX.uc);
}
/* Procedimiento que simula etapa MEM. Guarda o carga dato que está en memoria.*/
void MEM(Processor* myProcessor)
{
	char instruction[64];
	char rs[5];
	char rt[5];
	char rd[5];
	int inm;

	sscanf(myProcessor->instr4, "%s", instruction);
	strcpy(myProcessor->EX_MEM.inst, myProcessor->instr4);
	
	if(strcmp(instruction, "NONE") == 0 || strcmp(instruction, "NOP") == 0 || strcmp(instruction, "EOF") == 0 || strcmp(instruction, "FLUSH") == 0)
		return;

	copyBuffer(&myProcessor->MEM_WB, myProcessor->EX_MEM);

	if(strcmp(instruction, "sw") == 0)
	{
		int pointer = myProcessor->EX_MEM.ALUresult / 4;
		myProcessor->stack[pointer] = myProcessor->EX_MEM.readData2;
	}
	else if(strcmp(instruction, "lw") == 0)
	{
		int pointer = myProcessor->EX_MEM.ALUresult / 4;
		myProcessor->MEM_WB.loadData = myProcessor->stack[pointer];
	}
	myProcessor->MEM_WB.uc = copyUnitControl(myProcessor->EX_MEM.uc);
}
/* Procedimiento que simula etapa WB. Escribe en un registro de destino.*/
void WB(Processor* myProcessor)
{
	char instruction[64];

	sscanf(myProcessor->instr5, "%s", instruction);
	strcpy(myProcessor->EX_MEM.inst, myProcessor->instr5);

	if(strcmp(instruction, "NONE") == 0 || strcmp(instruction, "NOP") == 0 || strcmp(instruction, "EOF") == 0 || strcmp(instruction, "FLUSH") == 0)
		return;
	if(strcmp(instruction, "add") == 0 || strcmp(instruction, "sub") == 0 || strcmp(instruction, "mul") == 0 || strcmp(instruction, "div") == 0 || strcmp(instruction, "addi") == 0 || strcmp(instruction, "subi") == 0)
		myProcessor->registersData[getIndex(myProcessor->MEM_WB.MuxRegDst, myProcessor)] = myProcessor->MEM_WB.ALUresult;
	else if(strcmp(instruction, "lw") == 0)
		myProcessor->registersData[getIndex(myProcessor->MEM_WB.MuxRegDst, myProcessor)] = myProcessor->MEM_WB.loadData;
}
/* 	Función que retorna la instrucción, en caso de haber hazard de dato, de modo contrario retorna NULL
	myProcessor: Prcocesador al cual se quiere saber si existe hazard de dato.
	return: intrucción en la que hay hazard de control. */
char* hazardData(Processor* myProcessor)
{
	if(strcmp(myProcessor->EX_MEM.inst, "EOF") == 0 || strcmp(myProcessor->ID_EX.inst, "EOF") == 0 || 
	   strcmp(myProcessor->EX_MEM.inst, "NONE") == 0 || strcmp(myProcessor->ID_EX.inst, "NONE") == 0 ||
	   strcmp(myProcessor->EX_MEM.inst, "NOP") == 0 || strcmp(myProcessor->ID_EX.inst, "NOP") == 0 ||
	   strcmp(myProcessor->EX_MEM.inst, "FLUSH") == 0 || strcmp(myProcessor->ID_EX.inst, "FLUSH") == 0)
		return NULL;

	if(myProcessor->EX_MEM.uc.RegWrite == '1' && strcmp(myProcessor->EX_MEM.MuxRegDst, myProcessor->ID_EX.rs) == 0)
	{
		myProcessor->ID_EX.readData1 = myProcessor->EX_MEM.ALUresult;
		return myProcessor->ID_EX.rs;
	}
	if(myProcessor->EX_MEM.uc.RegWrite == '1' && strcmp(myProcessor->EX_MEM.MuxRegDst, myProcessor->ID_EX.rt) == 0)
	{
		myProcessor->ID_EX.readData2 = myProcessor->EX_MEM.ALUresult;
		return myProcessor->ID_EX.rt;
	}
	if(myProcessor->MEM_WB.uc.RegWrite == '1' && strcmp(myProcessor->MEM_WB.MuxRegDst, myProcessor->ID_EX.rs) == 0)
	{
		myProcessor->ID_EX.readData1 = myProcessor->MEM_WB.ALUresult;
		return myProcessor->ID_EX.rs;
	}
	if(myProcessor->MEM_WB.uc.RegWrite == '1' && strcmp(myProcessor->MEM_WB.MuxRegDst, myProcessor->ID_EX.rt) == 0)
	{
		myProcessor->ID_EX.readData2 = myProcessor->MEM_WB.ALUresult;
		return myProcessor->ID_EX.rt;
	}
	return NULL;
}
/*	Función que verifica si hay hazard de control.
	myProcessor: Procesador al cual se quiere sabe si hay hazard de control.
	return: instrucción en caso de que exista hazard de control.*/
char* hazardControl(Processor* myProcessor)
{
	if(strcmp(myProcessor->ID_EX.inst, "j") == 0)
		return "j";
	else if(strcmp(myProcessor->EX_MEM.inst, "beq") == 0 || myProcessor->stallB == 2)
		return "beq";
	else
		return NULL;
}
/* 	Procedimiento que realiza la impresión de los resultados a CSV.
	myFile: Archivo de destino de escritura.
	myList: Lista que se quiere imprimir.
	title: Convención de nombre representativo para la fila.*/ 
void printfListCSV(FILE* myFile, List* myList, char* title)
{
	int i;
	fprintf(myFile, "\n%s;", title);
	for(i = 0; i < myList->size; i++)
	{
		if(strcmp(myList->content[i], "EOF") == 0 || strcmp(myList->content[i], "NONE") == 0)
			fprintf(myFile, ";");
		else
			fprintf(myFile, "%s;", myList->content[i]);
	}
}
/*	Procedimiento que recopila los datos necesarios para efectuar la impresión de la salida.
	myProgram: Estructura que contiene los archivos de salida.
	myProcessor: Estructura que contiene los resultados. */
void printOut(Program* myProgram, Processor* myProcessor)
{
	// Impresión para csv de las etapas
	int i;
	fprintf(myProgram->traceFile, "ETAPAS;");
	for(i = 1; i < myProcessor->IFlist->size; i++)
		fprintf(myProgram->traceFile, "C%i;", i);
	printfListCSV(myProgram->traceFile, myProcessor->IFlist, "IF");
	printfListCSV(myProgram->traceFile, myProcessor->IDlist, "ID");
	printfListCSV(myProgram->traceFile, myProcessor->EXlist, "EX");
	printfListCSV(myProgram->traceFile, myProcessor->MEMlist, "MEM");
	printfListCSV(myProgram->traceFile, myProcessor->WBlist, "WB");
	fprintf(myProgram->hazardFile, "HAZARD;");
	for(i = 1; i < myProcessor->cycle; i++)
		fprintf(myProgram->hazardFile, "C%i;", i);
	printfListCSV(myProgram->hazardFile, myProcessor->hazardData, "DATO");
	printfListCSV(myProgram->hazardFile, myProcessor->hazardControl, "CONTROL");
	fclose(myProgram->traceFile);
	fclose(myProgram->hazardFile);
}
/*	Función que efectúa una espera
	myProcessor: Procesador al cual se realiza una espera.
	return: 0 si no hay espera, 1 si hay espera.*/
int NOP(Processor* myProcessor)
{
	if(myProcessor->ID_EX.uc.MemRead == '1' && (strcmp(myProcessor->ID_EX.rt, myProcessor->IF_ID.rs) == 0 || strcmp(myProcessor->ID_EX.rt, myProcessor->IF_ID.rt) == 0))
		return 1; // Hacer NOP
	return 0;
}
/*	Función que elimina contenido inservible de las listas.
	myList: Lista con valores innesesarios.
	return: Lista limpia.*/
List* removeTrash(List* myList)
{
	int i;
	int flag = 0;
	List* aux = initList();
	for(i = myList->size - 1; i >= 0; i--)
	{
		if(strcmp(myList->content[i], "NOP") == 0)
		{
			append(aux, myList->content[i]);
			flag = 1;
		}
		else if(flag == 0)
			append(aux, myList->content[i]);
		else
			flag = 0;
	}
	myList = initList();
	for(i = aux->size - 1; i >= 0; i--)
		append(myList, aux->content[i]);
	return myList;
}
/* 	Procedimiento que respalda los valores de los registros.
	myProcessor: Estructura que contiene valor de los registros.*/
void copyRegisters(Processor* myProcessor)
{
	int i;
	for(i = 0; i < 32; i++)
		myProcessor->previewData[i] = myProcessor->registersData[i];
}
/* Procedimiento que vuelve a lso datos respaldados.*/
void backRegisters(Processor* myProcessor)
{
	int i;
	for(i = 0; i < 32; i++)
		myProcessor->registersData[i] = myProcessor->previewData[i];
}
/*	Procedimiento que aplica FLUSH a la lista de salida de hazard de control.
	myProcessor: Procesador al cual se le aplciará el FLUSH. */
void applyFlush(Processor* myProcessor)
{
	int i;
	for(i = 0; i < myProcessor->hazardControl->size; i++)
	{
		if(strcmp(myProcessor->hazardControl->content[i], "beq") == 0)
		{
			strcpy(myProcessor->IFlist->content[i-2], "FLUSH");
			strcpy(myProcessor->IFlist->content[i-1], "FLUSH");
			strcpy(myProcessor->IDlist->content[i-1], "FLUSH");
			strcpy(myProcessor->IDlist->content[i], "FLUSH");
			strcpy(myProcessor->EXlist->content[i], "FLUSH");
			strcpy(myProcessor->EXlist->content[i+1], "FLUSH");
			strcpy(myProcessor->MEMlist->content[i+1], "FLUSH");
			strcpy(myProcessor->MEMlist->content[i+2], "FLUSH");
			strcpy(myProcessor->WBlist->content[i+2], "FLUSH");
			strcpy(myProcessor->WBlist->content[i+3], "FLUSH");
		}
		else if(strcmp(myProcessor->hazardControl->content[i], "j") == 0)
		{
			strcpy(myProcessor->IFlist->content[i-2], "FLUSH");
			strcpy(myProcessor->IDlist->content[i-1], "FLUSH");
			strcpy(myProcessor->EXlist->content[i], "FLUSH");
			strcpy(myProcessor->WBlist->content[i+1], "FLUSH");
		}
	}
}
/*	Procedimiento que realiza el comienzo del pipeline, finaliza imprimiendo el resultado de los datos. 
	myProgram: Estructura que contiene los archivos de entrada y salidas.*/
void start(Program* myProgram)
{
	Processor* myProcessor = initProcessor(myProgram);
	char* finish = (char*)malloc(sizeof(char)*64);
	char* hd = (char*)malloc(sizeof(char)*5);
	char* hc = (char*)malloc(sizeof(char)*5);
	int nop = 0;
	int flag = 0;
	do
	{
		printf("____________________\n");
		printf(ANSI_COLORBG_GREEN"Ciclo: %i\n"ANSI_COLOR_RESET, myProcessor->cycle);
		nop = NOP(myProcessor);
		if(nop == 1)
		{
			WB(myProcessor);
			MEM(myProcessor);
			EX(myProgram, myProcessor);
			ID(myProgram, myProcessor);
			append(myProcessor->hazardData, "-");
			append(myProcessor->hazardControl, "-");
			append(myProcessor->nopList, myProcessor->instr1);
			strcpy(myProcessor->instr1, "NOP");

			append(myProcessor->IFlist, myProcessor->instr1);
			append(myProcessor->IDlist, myProcessor->instr2);
			append(myProcessor->EXlist, myProcessor->instr3);
			append(myProcessor->MEMlist, myProcessor->instr4);
			append(myProcessor->WBlist, myProcessor->instr5);

			strcpy(finish, myProcessor->instr5);
			strcpy(myProcessor->instr5, myProcessor->instr4);
			strcpy(myProcessor->instr4, myProcessor->instr3);
			strcpy(myProcessor->instr3, myProcessor->instr2);
			strcpy(myProcessor->instr2, myProcessor->instr1);
			flag = 1;
			myProcessor->cycle++;
		}
		else if (flag == 1)
		{
			strcpy(myProcessor->instr1, pop(myProcessor->nopList));

			append(myProcessor->IFlist, myProcessor->instr1);
			append(myProcessor->IDlist, myProcessor->instr2);
			append(myProcessor->EXlist, myProcessor->instr3);
			append(myProcessor->MEMlist, myProcessor->instr4);
			append(myProcessor->WBlist, myProcessor->instr5);

			strcpy(finish, myProcessor->instr5);
			strcpy(myProcessor->instr5, myProcessor->instr4);
			strcpy(myProcessor->instr4, myProcessor->instr3);
			strcpy(myProcessor->instr3, myProcessor->instr2);
			strcpy(myProcessor->instr2, myProcessor->instr1);
			flag = 0;	
		}
		else if(myProcessor->stallB == 1 || myProcessor->stallB == 2)
		{
			char aux[5];
			if(myProcessor->stallB == 1)
				strcpy(aux, "j");
			else
				strcpy(aux, "beq");
			append(myProcessor->hazardControl, aux);
			append(myProcessor->hazardData, "-");
			
			myProcessor->cycle++;

			append(myProcessor->IFlist, myProcessor->instr1);
			append(myProcessor->IDlist, myProcessor->instr2);
			append(myProcessor->EXlist, myProcessor->instr3);
			append(myProcessor->MEMlist, myProcessor->instr4);
			append(myProcessor->WBlist, myProcessor->instr5);

			strcpy(finish, myProcessor->instr5);
			strcpy(myProcessor->instr5, myProcessor->instr4);
			strcpy(myProcessor->instr4, myProcessor->instr3);
			strcpy(myProcessor->instr3, myProcessor->instr2);
			strcpy(myProcessor->instr2, myProcessor->instr1);
			myProcessor->uc = initUnitControl();
			myProcessor->IF_ID = initBuffer();
			myProcessor->ID_EX = initBuffer();
			myProcessor->EX_MEM = initBuffer();
			myProcessor->MEM_WB = initBuffer();
			myProcessor->stallB = 0;
		}
		else 
		{
			WB(myProcessor);
			MEM(myProcessor);
			EX(myProgram, myProcessor);
			ID(myProgram, myProcessor);
			IF(myProgram, myProcessor);	

			append(myProcessor->IFlist, myProcessor->instr1);
			append(myProcessor->IDlist, myProcessor->instr2);
			append(myProcessor->EXlist, myProcessor->instr3);
			append(myProcessor->MEMlist, myProcessor->instr4);
			append(myProcessor->WBlist, myProcessor->instr5);

			hd = hazardData(myProcessor);

			strcpy(finish, myProcessor->instr5);
			strcpy(myProcessor->instr5, myProcessor->instr4);
			strcpy(myProcessor->instr4, myProcessor->instr3);
			strcpy(myProcessor->instr3, myProcessor->instr2);
			strcpy(myProcessor->instr2, myProcessor->instr1);
			if (strcmp(finish, "EOF") != 0)
			{
				if(hd == NULL)
					append(myProcessor->hazardData, "-");
				else
					append(myProcessor->hazardData, hd);
				append(myProcessor->hazardControl, "-");
				myProcessor->cycle++;
			}
		}
		printf("\n");
		printf("----------------\n");
		printf("\n>IF:  %s\n", myProcessor->instr1);
		printf(">ID:  %s\n", myProcessor->instr2);
		printf(">EX:  %s\n", myProcessor->instr3);
		printf(">MEM: %s\n", myProcessor->instr4);
		printf(">WB:  %s\n", myProcessor->instr5);
	}
	while(strcmp(finish, "EOF") != 0);
	myProcessor->IFlist = removeTrash(myProcessor->IFlist);
	myProcessor->IDlist = removeTrash(myProcessor->IDlist);
	myProcessor->EXlist = removeTrash(myProcessor->EXlist);
	myProcessor->MEMlist = removeTrash(myProcessor->MEMlist);
	myProcessor->WBlist = removeTrash(myProcessor->WBlist);
	applyFlush(myProcessor);
	printOut(myProgram, myProcessor);
	fclose(myProgram->instFile);
	fclose(myProgram->registerFile);
} 