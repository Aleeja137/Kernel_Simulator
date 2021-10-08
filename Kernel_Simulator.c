#include <stdio.h>
#include <stdlib.h>

//Construye un programa en C que tome dos parámetros: param1 y param2. Ambos son números
//enteros con valores entre 1 y 4:

int main (int argc, char *argv[]) {

//Control de que se ha pasado mínimamente un argumento
if (argc==3){printf("Necesita tres argumentos");exit(1);}

int fil = atoi(argv[1]);
int col = atoi(argv[2]);

//Control de que el argumento es válido
if (fil<1 || col<1 || fil>4 || col>4) {
    printf("Su número se sale de los parámetros \n");
    exit (1);
} else {
    printf("Siga, su número de filas es: %d  y su número de columnas es: %d\n", fil,col);
}

int filMax = 1 << fil;
int colMax = 1 << col;
printf("El máximo número de filas es: %d  y el máximo número de columnas es: %d\n", fil,col);

//Crear array y llenarlo de letras minúsculas
char letras[filMax][colMax];
int contador = 0;
char newChar;
for (int i = 0; i < filMax; i++)
{
    for (int j = 0; j < colMax; j++)
    {
        newChar = (0x61 + (contador%26));
        letras[i][j]=newChar;
        printf("En la posicion [%d,%d] he puesto el carácter %c \n" ,i,j,newChar);

    }
}

printf("_____________________________________________ \n \n");

for (int i = 0; i < filMax; i++)
{
    for (int j = 0; j < colMax; j++)
    {
        letras[i][j] &= ~(1<<5);
        printf("En la posicion [%d,%d] está el carácter %c \n" ,i,j,newChar);

    }
}
return 1;
}