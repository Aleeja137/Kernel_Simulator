#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "data_structures.h"
#include "loader.h"
#include <sys/stat.h>
#include <string.h>

 void* loader_func_prueba(void * argument){
    printf("I am the loader\n");
    pthread_exit(NULL);
}

void* loader_function(void* args){
    pthread_mutex_lock(&mtxpGenerator);
    int ultimoid = 0;
    while (1)
    {        
        // CONTROL : printf("Llamado al loader desde el temporizador\n");
        done_countpGenerator++;
        
        //------------Leer los datos necesarios------------------//

        //Prepara el nombre del archivo, puede ser dentro del rango 000-999
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

        // CONTROL : printf("El nombre es %s\n",code);
        //Abre el archivo y mira cuántos frames son necesarios
        FILE* programFile;

        programFile = fopen(code,"r");
        if (programFile == NULL)
        {
            printf("No hay más archivos que leer\n");
        } else {
            struct stat programInfo;
            stat(code,&programInfo);
            int framesProceso = (((programInfo.st_size-26)/9)/64); //He comprobado que un archivo con las dos líneas iniciales y el último salto de línea son 26 bytes, y por cada línea de instrucciones y el salto de línea son 9 bytes
            if (((programInfo.st_size-26)/9)%64)
            {
                framesProceso++; //Esto es para redondear para arriba, he visto que es común usar (x+(y-1))/y para redondear para arriba pero como no lo veo del todo me quedo con esto
            }
            //Construye el mm
            mm_t new_mm;

            new_mm.pgb = (unsigned int*)malloc(framesProceso*sizeof(unsigned int));
            if(lista_frames.free_frames<framesProceso){
                printf("Insuficiente espacio en memoria para el proceso\n");
            } else {
                //Lee línea por línea y guarda cada cosa en su lugar
                char read[14];      //Comprobado que son 14 bytes los cabecera
                char valores[6];    //Comprobado que son los bytes 6-11
                mm_t new_mm;
                new_mm.num_frames = framesProceso;
                int i = 0;
                int frameActual = 0;
                unsigned int dondeGuardar = 0x00;
                int instruccionesYaLeidas = 0;

                //Reservar memoria para las tablas, y asignar las direcciones de los frames libres
                int frameActual2 = 0;
                for(int i=0; i<framesProceso;i++){
                    new_mm.pgb = (int*)malloc(framesProceso*sizeof(int)); //Reservar memoria para la pagina de tablas del PCB
                }
                new_mm.pgb[frameActual2] = (lista_frames.first->address);
                dondeGuardar = lista_frames.first->address;
                lista_frames.first = lista_frames.first->next;
                while (fgets(read,14,programFile))
                {             
                    printf("el dato leído es: %s\n",read);
                    memcpy(valores,&read[6],6); // CONTROL : Antes estaba puesto a copiar 7 bytes, que funcionaba, pero a saber qué pasa ahora
                    if (i == 1) //La segunda línea nos dice dónde empieza el código, anotar
                    {
                        new_mm.code = (int)strtol(valores,NULL,16);  // (NOTA) He probado con stroul pero no me funciona bien siempre, no sé usarlo bien
                    } else if (i == 0) //La primera nos dice el dato, anotar
                    {
                        new_mm.data = (int)strtol(valores,NULL,16);
                    } else // A partir de ahí guardar todo en memoria física
                    {
                        //Si hemos leído un múltiplo de 256 de líneas reservamos otro frame
                        if (instruccionesYaLeidas%256 == 0)  
                        {
                            frameActual++;
                            new_mm.pgb[frameActual] = lista_frames.first->address;
                            dondeGuardar = lista_frames.first->address;
                            lista_frames.first = lista_frames.first->next;
                        }
                        //Guardamos en memoria física los datos
                        unsigned int linea = (int)strtol(read,NULL,16);
                        memcpy(&memoria_fisica[dondeGuardar],&linea,8);   // CONTROL : Puede que falle esto (?)
                        dondeGuardar+=32;
                        instruccionesYaLeidas+=4;
                    } 
                    i++;
                }
                // Crear el nuevo proceso
                process_info_t new_PCB;
                new_PCB.mm = new_mm;
                new_PCB.pid = ultimoid;
                new_PCB.status = 0;

                //Meterlo en la cola de procesos para el dispacher
                process_node_t *nodo = (process_node_t*)malloc(sizeof(process_node_t));
                nodo->me = new_PCB;
                nodo->next = NULL;

                if (lista_procesos.first == NULL)
                {
                    lista_procesos.first = nodo;
                } else {
                    lista_procesos.last->next = nodo;
                }
                lista_procesos.last = nodo;

                 printf("Creado el proceso %d teniendo data en %d y text en %d\n",
                        ultimoid,
                        new_PCB.mm.code,
                        new_PCB.mm.data);
                
            }
            fclose(programFile);
            ultimoid++;

        }
        
        pthread_cond_signal(&condpGenerator);
        pthread_cond_wait(&cnd_brpGenerator,&mtxpGenerator);
    }
}