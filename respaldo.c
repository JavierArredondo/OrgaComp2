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

void changeBuffers(Processor* myProcessor)
{
	myProcessor->MEM_WB = myProcessor->EX_MEM;
	myProcessor->MEM_WB.loadData = myProcessor->AUX.loadData;

	myProcessor->EX_MEM = myProcessor->ID_EX;
	strcpy(myProcessor->EX_MEM.MuxRegDst, myProcessor->AUX.MuxRegDst);
	myProcessor->EX_MEM.ALUresult = myProcessor->AUX.ALUresult;
	myProcessor->EX_MEM.zero = myProcessor->AUX.zero;

	myProcessor->ID_EX = myProcessor->IF_ID;
	strcpy(myProcessor->ID_EX.rs, myProcessor->AUX.rs);
	strcpy(myProcessor->ID_EX.rt, myProcessor->AUX.rt);
	strcpy(myProcessor->ID_EX.rd, myProcessor->AUX.rd);
	strcpy(myProcessor->ID_EX.labelBranch, myProcessor->AUX.labelBranch);
	myProcessor->ID_EX.uc = myProcessor->uc;
	myProcessor->ID_EX.signExtend = myProcessor->AUX.signExtend;
	myProcessor->ID_EX.readData1 = myProcessor->AUX.readData1;
	myProcessor->ID_EX.readData2 = myProcessor->AUX.readData2;

	myProcessor->IF_ID = initBuffer();

	strcpy(myProcessor->IF_ID.inst, myProcessor->instr1);
}

void moveBuffer(Processor* myProcessor)
{
	//MEM_WB
	myProcessor->MEM_WB.uc.RegDst =   myProcessor->EX_MEM.uc.RegDst;;
	myProcessor->MEM_WB.uc.Jump =     myProcessor->EX_MEM.uc.Jump;
	myProcessor->MEM_WB.uc.Branch =   myProcessor->EX_MEM.uc.Branch;
	myProcessor->MEM_WB.uc.MemRead =  myProcessor->EX_MEM.uc.MemRead;
	myProcessor->MEM_WB.uc.MemToReg = myProcessor->EX_MEM.uc.MemToReg;
	myProcessor->MEM_WB.uc.MemWrite = myProcessor->EX_MEM.uc.MemWrite;
	myProcessor->MEM_WB.uc.ALUSrc =   myProcessor->EX_MEM.uc.ALUSrc;
	myProcessor->MEM_WB.uc.RegWrite = myProcessor->EX_MEM.uc.RegWrite;

	//EX_MEM
	myProcessor->EX_MEM.uc.RegDst =   myProcessor->ID_EX.uc.RegDst;;
	myProcessor->EX_MEM.uc.Jump =     myProcessor->ID_EX.uc.Jump;
	myProcessor->EX_MEM.uc.Branch =   myProcessor->ID_EX.uc.Branch;
	myProcessor->EX_MEM.uc.MemRead =  myProcessor->ID_EX.uc.MemRead;
	myProcessor->EX_MEM.uc.MemToReg = myProcessor->ID_EX.uc.MemToReg;
	myProcessor->EX_MEM.uc.MemWrite = myProcessor->ID_EX.uc.MemWrite;
	myProcessor->EX_MEM.uc.ALUSrc =   myProcessor->ID_EX.uc.ALUSrc;
	myProcessor->EX_MEM.uc.RegWrite = myProcessor->ID_EX.uc.RegWrite;
}
