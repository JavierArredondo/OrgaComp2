# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "structs.h"
# include "functions.c"
int main()
{
	Program* myProgram;
	//Processor* myProcessor;
	char nameInstruction[40];
	char nameRegister[40];
	char nameOutFile[40];
	FILE* fileInstr;
	FILE* fileControlLine;
	FILE* fileOut;
	FILE* fileOut2;
	int option;
	do
	{
		printf(ANSI_COLOR_CYAN "\nLABORATORIO 2: Hazard\n" ANSI_COLOR_RESET);
		printf("\nEscoja una opcion:\n1.- Ejecutar programa.\n2.- Creditos\n3.- Instrucciones\n4.- Salir\n");
		scanf("%i", &option);
		if(option == 1)
		{
			system("cls");
			/* Primero se pide un archivo válido para la ejecución del programa, si este no existe, se pedrirá un nuevo archivo. */
			printf("\nIngrese el nombre del archivo que posee las instrucciones del programa: ");
			scanf("%s", nameInstruction);
			fileInstr = inDirectory("code.asm");
			while(!fileInstr)
			{
				printf(ANSI_COLOR_RED "\nNo existe tal archivo. Ingrese uno valido: " ANSI_COLOR_RESET);
				scanf("%s", nameInstruction);
				fileInstr = inDirectory(nameInstruction);
			}
			/*Luego se pide el segundo archivo, se repite el proceso anterior si es que este no existe. */
			printf("Ingrese el nombre del archivo que posee los registros: ");
			scanf("%s", nameRegister);
			fileControlLine = inDirectory("inputRegistros.txt");
			while(!fileControlLine)
			{
				printf(ANSI_COLOR_RED "\nNo existe tal archivo. Ingrese uno valido: " ANSI_COLOR_RESET);
				scanf("%s", nameRegister);
				fileControlLine = inDirectory(nameRegister);
			}
			printf("Ingrese el nombre del archivo de salida que desee (sin extension): ");
			scanf("%s", nameOutFile);
			printf("\n");
			strcat(nameOutFile,".csv");
			fileOut = fopen("asd.csv", "w");
			fileOut2 = fopen("asd2.csv", "w");
			myProgram = initProgram(fileInstr, fileControlLine, fileOut, fileOut2);
			start(myProgram);
			//myProcessor = initProcessor(myProgram);
			//readMips(myProgram, myProcessor);	
		}
		else if(option == 2)
		{
			printf(ANSI_COLOR_CYAN "\nConstruido por:\n" ANSI_UNDERLINE_OFF);
			printf(ANSI_COLOR_CYAN "       _             _                                        _                 _       \n" ANSI_COLOR_RESET);
			printf(ANSI_COLOR_CYAN "      | |           (_)               /\\                     | |               | |      \n" ANSI_COLOR_RESET);
			printf(ANSI_COLOR_CYAN "      | | __ ___   ___  ___ _ __     /  \\   _ __ _ __ ___  __| | ___  _ __   __| | ___  \n" ANSI_COLOR_RESET);
			printf(ANSI_COLOR_CYAN "  _   | |/ _` \\ \\ / / |/ _ \\ '__|   / /\\ \\ | '__| '__/ _ \\/ _` |/ _ \\| '_ \\ / _` |/ _ \\ \n" ANSI_COLOR_RESET);
			printf(ANSI_COLOR_CYAN " | |__| | (_| |\\ V /| |  __/ |     / ____ \\| |  | | |  __/ (_| | (_) | | | | (_| | (_) |\n" ANSI_COLOR_RESET);
			printf(ANSI_COLOR_CYAN "  \\____/ \\__,_| \\_/ |_|\\___|_|    /_/    \\_\\_|  |_|  \\___|\\__,_|\\___/|_| |_|\\__,_|\\___/ \n" ANSI_COLOR_RESET);
			printf(ANSI_COLOR_CYAN "                         Univesidad de Santiago de Chile - 2017\n" ANSI_COLOR_RESET);
		}
		else if(option == 3)
		{
			printf("\nEl programa consiste en evaluar un programa escrito en lenguaje ensamblador MIPS, este debe ser capaz de verificar ");
			printf("si dicho programa es congruente con ciertas lineas de control. Para esto se debe realizar el siguiente procedimiento:\n");
			printf("\t1) Ingresar el nombre del archivo MIPS (con su extension [.asm])\n");
			printf("\t2) Ingresar el nombre del archivo que posee las lineas de control (con su extension [.txt])\n");
			printf("\t3) Ingresar el nombre del archivo de salida (sin su extension)\n\n");
		}
		else if(option == 4)
			break;

		else
			printf("\nCUIDADO: Opcion no valida.\n");
	}
	while(option !=4);
	return 0;
}