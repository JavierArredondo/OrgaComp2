# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "structs.h"
//# include "functions.h"
# include "functions.c"

int main()
{
	Program* myProgram;
	char nameInstruction[40];
	char nameControlLine[40];
	char nameOutFile[40];
	FILE* fileInstr;
	FILE* fileControlLine;
	FILE* fileOut;
	int option;
	do
	{
		printf("\nLABORATORIO 1: Camino de datos\n\nEscoja una opcion:\n1.- Ejecutar programa.\n2.- Creditos\n3.- Instrucciones\n4.- Salir\n");
		scanf("%i", &option);
		if(option == 1)
		{
			/* Primero se pide un archivo válido para la ejecución del programa, si este no existe, se pedrirá un nuevo archivo. */
			printf("\nIngrese el nombre del archivo que posee las instrucciones del programa: ");
			scanf("%s", nameInstruction);
			fileInstr = inDirectory(nameInstruction);
			while(!fileInstr)
			{
				printf("\nNo existe tal archivo. Ingrese uno valido: ");
				scanf("%s", nameInstruction);
				fileInstr = inDirectory(nameInstruction);
			}	
		}
		else if(option == 2)
		{
		printf("\nConstruido por:\n");
			printf("       _             _                                        _                 _       \n");
			printf("      | |           (_)               /\\                     | |               | |      \n");
			printf("      | | __ ___   ___  ___ _ __     /  \\   _ __ _ __ ___  __| | ___  _ __   __| | ___  \n");
			printf("  _   | |/ _` \\ \\ / / |/ _ \\ '__|   / /\\ \\ | '__| '__/ _ \\/ _` |/ _ \\| '_ \\ / _` |/ _ \\ \n");
			printf(" | |__| | (_| |\\ V /| |  __/ |     / ____ \\| |  | | |  __/ (_| | (_) | | | | (_| | (_) |\n");
			printf("  \\____/ \\__,_| \\_/ |_|\\___|_|    /_/    \\_\\_|  |_|  \\___|\\__,_|\\___/|_| |_|\\__,_|\\___/ \n");
			printf("                         Univesidad de Santiago de Chile - 2017\n");
		}
		else if(option == 3)
		{
			printf("\nEl programa consiste en evaluar un programa escrito en lenguaje ensamblador MIPS, este debe ser capaz de verificar ");
			printf("si dicho programa es congruente con ciertas lineas de control. Para esto se debe realizar el siguiente procedimiento:\n");
			printf("\ta) Ingresar el nombre del archivo MIPS (con su extension [.asm])\n");
			printf("\tb) Ingresar el nombre del archivo que posee las lineas de control (con su extension [.txt])\n");
			printf("\tc) Ingresar el nombre del archivo de salida (sin su extension)\n\n");
		}
		else if(option == 4)
			break;

		else
			printf("\nCUIDADO: Opcion no valida.\n");
	}
	while(option !=4);
	return 0;
}