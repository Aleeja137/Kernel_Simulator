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
<<<<<<< HEAD
int num_timers;
int done_count;
int* frecuencias;

pthread_t* id; 
pthread_cond_t* condiciones;
int cond_clock; 
pthread_mutex_t mutex_clock;
int cond_clock_broadcast;
=======
>>>>>>> parent of 5a7dd54 (Updates)

machine_t machine;

//COSAS PARA HACER EN GENERAL
//La comunicación entre hilos mediante mutex_t y cond_t
//Aprender a mover los headers a otra carpeta y que compile bien

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
       printf("la inicialización ha sido un éxito!!! \n");
   } else {
       printf("algo falla en la inicialización :( \n");
   }

    
    pthread_t id[4]; //Array que guarda los ID de los threads para el clock, timer, dispacher y PGenerator
    //id[0] clock
    //id[1] dispacher
    //id[2] PGenerator
    //id[3] timer

    int aux=0;
    
    pthread_create(&id[0],NULL,clock_func_prueba,&aux);
    pthread_create(&id[1],NULL,schedule_dispacher_func_prueba,&aux);
    pthread_create(&id[2],NULL,process_generator_func_prueba,&aux);
<<<<<<< HEAD
    
    for (int i = 0; i < num_timers; i++)
    {
        pthread_create(&id[3+i],NULL,timer_function,i);
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
int inicializar_control()
{
    condiciones = (pthread_cond_t*)malloc(2*sizeof(pthread_cond_t));
    pthread_cond_init(&condiciones[0],NULL); 
    cond_clock = 0;
    pthread_cond_init(&condiciones[1],NULL);
    cond_clock_broadcast = 1;

}

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
    inicializar_control();
    crear_hilos(frecuencias);




    eliminar_hilos();
=======
    pthread_create(&id[3],NULL,timer_func_prueba,&aux);
>>>>>>> parent of 5a7dd54 (Updates)

    pthread_join(id[0],NULL);
    pthread_join(id[1],NULL);
    pthread_join(id[2],NULL);
    pthread_join(id[3],NULL);
    return 1;
}
