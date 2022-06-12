#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include "data_structures.h"
#include <stdlib.h>



int main(int argc, char** argv)
{
    /*
    int ultimoid = atoi(argv[1]);
    char code[21] = {"Programas/prog000.elf"};
    int resto = 0;
    int resultado = ultimoid;
    //ultimoid = xyz
    resto = resultado % 10;   //resto = xyz mod 10 = z
    resultado = resultado/10; //resultado = xyz div 10 = xy
    code[16] = '0' + resto;  //prog00z
    resto = resultado % 10;  //resto = xy mod 10 = y
    resultado = resultado/10; //resultado = xy div 10 = x
    code[15] = '0' + resto;  //prog0yz
    code[14] = '0' + resultado; //progxyz

    printf("The value of the char sequence in general is %s\n",code);    

    //---------------------------------//

    struct stat info;
    stat("/home/alejandro/Desktop/Universidad/SO/Kernel_Simulator/Programas/prog050.elf",&info);
    printf("Size in bytes is %ld\n",info.st_size); //Sólo con la cabecera da 26 y por línea da 9
    */

    //---------------------------------//

    FILE* programa;
    programa = fopen("Programas/prog050.elf","r");
    char read[13];
    fgets(read,13,programa); //Con 13 lee hasta antes del salto de línea
    printf("%s\n",read);
    //printf("%d\n",read[11]); //Desde el byte 6 al 11 es la dirección


    char valores[6];    //Comprobado que son los bytes 6-11
    memcpy(valores,&read[6],7); //Por alguna razón con N=6 no me funciona bien, pero con N>6 sí, así que lo dejo con 7
    printf("Estos son los valores: %s agur\n",valores);

    //---------------------------------//

    /*
    mm_t new_mm;
    new_mm.code = (int)strtol(valores, NULL, 16);
    printf("%d\n",new_mm.code);

    //---------------------------------//

    FILE* programa;
    programa = fopen("/home/alejandro/Desktop/Universidad/SO/Kernel_Simulator/Programas/prog050.elf","r");
    char read[14]; //Comprobado que son 14 bytes los cabecera
    char valores[6];    //Comprobado que son los bytes 6-11
    mm_t new_mm;
    fgets(read,14,programa);
    memcpy(valores,&read[6],7);
    new_mm.code = (int)strtol(valores,NULL,16);
    fgets(read,14,programa);
    memcpy(valores,&read[6],7);
    new_mm.data = (int)strtol(valores,NULL,16);

    printf("%d\n",new_mm.code);
    printf("%d\n",new_mm.data);
    

    //-----------------------------------//

    //Quiero hacer lo mismo con un while
    FILE* programa;
    programa = fopen("/home/alejandro/Desktop/Universidad/SO/Kernel_Simulator/Programas/prog050.elf","r");
    char read[14]; //Comprobado que son 14 bytes los cabecera
    char valores[6];    //Comprobado que son los bytes 6-11
    mm_t new_mm;
    int i = 0;
    int frameActual;
    while (fgets(read,14,programa))
    {
        memcpy(valores,&read[6],7);
        if (i == 0)
        {
            new_mm.code = (int)strtol(valores,NULL,16);
        } else if (i == 1)
        {
            new_mm.data = (int)strtol(valores,NULL,16);
        } else // Ir guardando los datos en memoria 
        {

        }
        //Falta añadir que vaya asignando frames y guardando los datos en memoria física
        
        i++;
    }
    
    printf("%d\n",new_mm.code);
    printf("%d\n",new_mm.data);
    */
} 