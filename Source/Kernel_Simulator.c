#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "data_structures.h"
#include "globals.h"

//Variables globales
int num_CPUs;
int num_cores;
int num_processes;

machine_t machine;

//COSAS PARA HACER EN GENERAL
//La comunicación entre hilos mediante mutex_t y cond_t

//COSAS PARA HACER AQUI
//Inicializar estructuras (los de data_structures con los parámetros recibidos) (DONE)
//Llamar a los pthreads y asignar un papel a cada

int inicializar(int argc, char *argv[])
{
    if (argc<4) {
        printf("Not enough arguments");
        exit(-1);
    }
    num_CPUs = atoi(argv[1]);
    printf("Number of CPUs is %d \n",num_CPUs);
    num_cores = atoi(argv[2]);
    printf("Number of cores is %d \n",num_cores);
    num_processes = atoi(argv[3]);
    printf("Number of threads is %d \n",num_processes);

    //inicializar la máquina
    int cpu_id_aux=0;
    int core_id_aux=0;
    machine.cpu_array = (cpu_t*)malloc(num_CPUs*sizeof(cpu_t)); //Se guarda espacio para las CPUs
    for (int i = 0; i < num_CPUs; i++)  //Recorre el array de CPUs asignando un identificador y guardando espacio para los cores
    {
        machine.cpu_array[i].cpu_id = cpu_id_aux;
        machine.cpu_array[i].core_array = (core_t*)malloc(num_cores*sizeof(core_t));
        printf("created cpu number %d \n",cpu_id_aux);
        for (int j = 0; j < num_cores; j++)  //Recorre los cores dentro de cada CPU asignando identificador y guardando espacio para los hilos
        {
            machine.cpu_array[i].core_array[j].core_id = core_id_aux;
            machine.cpu_array[i].core_array[j].process_array = (process_info_t*)malloc(num_processes*sizeof(process_info_t));           
            printf("created core number %d \n",core_id_aux);
            core_id_aux++;
        }
        cpu_id_aux++; 
    }
    return 1;
}

int main(int argc, char *argv[]) {
   if (inicializar(argc,argv)==1)
   {
       printf("la inicialización ha sido un éxito!!!");
   } else {
       printf("algo falla en la inicialización :(");
   }

    #ifdef TIMING
    pthread_t id[4]; //Array que guarda los ID de los threads para el clock, timer, dispacher y PGenerator
    
    pthread_create(id[0],NULL,clock,NULL);
    #endif
    return 1;
}
