# include "structs.h"

#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

/* Función que verifica si un archivo de entrada existe en el directorio. */
FILE* inDirectory(char* name);

char** initRegisterName();

int* initRegisterData(Program* myProgram);

Processor* initProcessor(Program* myProgram);

Program* initProgram(FILE* firstFile, FILE* secFile, FILE* thirdFile, FILE* fourthFile);

Buffer initBuffer();

List* initList();

void append(List* myList, char* element);

int getIndex(char* myRegister, Processor* myProcessor);

void goToLabel(Program* myProgram, Processor* myProcessor, char* myLabel);

void printRegisters(Processor* myProcessor);

void changeUnitControl(char a1, char a2, char a3, char a4, char a5, char* a6, char a7, char a8, char a9, UnitControl* myUniControl);

void setUnitControl(char* instr, Processor* myProcessor);

void IF(Program* myProgram, Processor* myProcessor);

void ID(Program* myProgram, Processor* myProcessor);

int forwarding(Processor* myProcessor);
#endif