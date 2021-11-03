#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "data_structures.h"
#include "globals.h"
#include "clock.h"
#include "schedule_dispacher.h"
#include "process_generator.h"
#include "timer.h"

//Variables globales
int num_CPUs;
int num_cores;
int num_processes;
int num_timers;
int done_count;
int* frecuencias;

pthread_t* id; 
pthread_cond_t cond_clock; 
pthread_mutex_t mutex_clock;
pthread_cond_t cond_clock_broadcast;

machine_t machine;

//COSAS PARA HACER EN GENERAL
//La comunicación entre hilos mediante mutex_t y cond_t
//Aprender a mover los headers a otra carpeta y que compile bien

//COSAS PARA HACER AQUI
//(DONE) Inicializar estructuras (los de data_structures con los parámetros recibidos)
//(DONE) Llamar a los pthreads y asignar un papel a cada

int inicializar_hardware()
{
    //printf("Number of CPUs is %d \n",num_CPUs);
    //printf("Number of cores is %d \n",num_cores);
    //printf("Number of threads is %d \n",num_processes);

    //inicializar la máquina
    int cpu_id_aux=0;
    int core_id_aux=0;
    machine.cpu_array = (cpu_t*)malloc(num_CPUs*sizeof(cpu_t)); //Se guarda espacio para las CPUs
    for (int i = 0; i < num_CPUs; i++)  //Recorre el array de CPUs asignando un identificador y guardando espacio para los cores
    {
        machine.cpu_array[i].cpu_id = cpu_id_aux;
        machine.cpu_array[i].core_array = (core_t*)malloc(num_cores*sizeof(core_t));
        //printf("created cpu number %d \n",cpu_id_aux);
        for (int j = 0; j < num_cores; j++)  //Recorre los cores dentro de cada CPU asignando identificador y guardando espacio para los hilos
        {
            machine.cpu_array[i].core_array[j].core_id = core_id_aux;
            machine.cpu_array[i].core_array[j].process_array = (process_info_t*)malloc(num_processes*sizeof(process_info_t));           
            //printf("created core number %d \n",core_id_aux);
            core_id_aux++;
        }
        cpu_id_aux++; 
    }
    return 1;
}
int crear_hilos(int* frecuencias)
{
    //id[0] clock
    //id[1] dispacher
    //id[2] PGenerator
    //id[3+] timer

    id = (pthread_t*)malloc((3+num_timers)*sizeof(pthread_t));

    int aux=0;
    
    pthread_create(&id[0],NULL,clock_function,&aux);
    pthread_create(&id[1],NULL,schedule_dispacher_func_prueba,&aux);
    pthread_create(&id[2],NULL,process_generator_func_prueba,&aux);
    
    for (int i = 0; i < num_timers; i++)
    {
        pthread_create(&id[3+i],NULL,timer_function,(void*)&i);
    }

    return 1;
}
int eliminar_hilos()
{
    for (int i = 0; i < 3+num_timers; i++)
    {
        pthread_join(id[i],NULL);
    }
}
//inicializar_mutex(s)

int main(int argc, char *argv[]) {

   if (argc<5) {
        printf("Not enough arguments");
        exit(-1);
    }
    num_CPUs = atoi(argv[1]);
    num_cores = atoi(argv[2]);
    num_processes = atoi(argv[3]);

    num_timers = atoi(argv[4]);
    frecuencias = (int*)malloc((num_timers)*sizeof(int));
    for (int j = 0; j < num_timers; j++)
    {
        frecuencias[j] = atoi(argv[5+j]);
    }
    
    inicializar_hardware();
    crear_hilos(frecuencias);




    eliminar_hilos();

    return 1;
}
