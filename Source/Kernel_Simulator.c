#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "data_structures.h"
#include "globals.h"
#include "clock.h"
#include "schedule_dispacher.h"
#include "loader.h"
#include "timer.h"

//Variables globales
int num_CPUs;
int num_cores;
int num_processes;
int num_timers;
int* frecuencias;
pthread_t* id; 

int done_count;
pthread_mutex_t mtx; 
pthread_cond_t cond; 
pthread_cond_t cnd_br;

int done_countScheduler;
pthread_mutex_t mtxScheduler; 
pthread_cond_t condScheduler; 
pthread_cond_t cnd_brScheduler;

int done_countpGenerator;
pthread_mutex_t mtxpGenerator; 
pthread_cond_t condpGenerator; 
pthread_cond_t cnd_brpGenerator;

machine_t machine;
process_queue_t lista_procesos;

// Variables nuevas y/o cambiadas

unsigned char* memoria_fisica;
frames_list lista_frames;


int inicializar_hardware()
{
    //Reservar memoria
    unsigned int memory_space = 16777216;
    memoria_fisica = (char*)malloc(memory_space);

    //Inicializar lista de frames disponibles en memoria física
    unsigned int base = 0x400000; //La memoria física tiene ocupado hasta aquí por el propio kernel

    while (base < 0xFFFFFC) //Hasta llegar al final de la memoria disponible
    {
        //Creamos un nuevo hueco y lo inicializamos
        frame_t* frame = (frame_t*)malloc(sizeof(frame_t)); 
        frame->next = NULL;
        frame->address = base;

        //Meterlo en la cola
        if (lista_frames.first == NULL){  //La lista está vacía
            lista_frames.first = frame;
            lista_frames.last = frame;
            lista_frames.free_frames = 1;
        } else {                          //Se actualiza el último y se suma la cantidad de frames libres en la memoria
            lista_frames.last->next = frame;
            lista_frames.last = frame;
            lista_frames.free_frames ++ ;
        }

        base += 256; //Se incrementa en 256B, y como la memoria se direcciona al byte, en 256 posiciones, ya que una página tiene esos bytes 
    }

    // Para este punto tenemos la memoria física, y la lista de frames libres de esa memoria (quitando el hueco que ocuparía el kernel)
    // Ahora se debe inicializar el hardware

    
    // Crear mm, PCB, TLB, MMU y HwThread vacíos
    mm_t mm_vacio;
    mm_vacio.code = 0;
    mm_vacio.data = 0;
    mm_vacio.pgb = 0;
    //mm_vacio.num_frames = 0;

    process_info_t proceso_vacio;
    proceso_vacio.pid = -1;
    proceso_vacio.status = -1;
    proceso_vacio.mm = mm_vacio;

    TLB_t TLB_vacio;
    TLB_vacio.physical = 0;
    TLB_vacio.virtual = 0;

    MMU_t MMU_vacio;
    MMU_vacio.TLB[0] = TLB_vacio;
    MMU_vacio.TLB[1] = TLB_vacio;
    MMU_vacio.TLB[2] = TLB_vacio;
    MMU_vacio.TLB[3] = TLB_vacio;

    HwThread_t hilo_vacio;
    hilo_vacio.IR = 0;
    hilo_vacio.PC = 0;
    hilo_vacio.MMU = MMU_vacio;
    hilo_vacio.PTBR = 0;
    hilo_vacio.proceso = proceso_vacio;

    int register_aux = 0;
    for(register_aux = 0; register_aux<16; register_aux++){
        hilo_vacio.registros[register_aux] = 0;
    }

    //inicializar la máquina
    int cpu_id_aux = 0;
    int core_id_aux = 0;
    machine.cpu_array = (cpu_t*)malloc(num_CPUs*sizeof(cpu_t)); //Se guarda espacio para las CPUs
    for (int i = 0; i < num_CPUs; i++)  //Recorre el array de CPUs asignando un identificador y guardando espacio para los cores
    {
        machine.cpu_array[i].cpu_id = cpu_id_aux;
        machine.cpu_array[i].core_array = (core_t*)malloc(num_cores*sizeof(core_t));
        cpu_id_aux++; 
        //printf("created cpu number %d \n",cpu_id_aux);
        for (int j = 0; j < num_cores; j++)  //Recorre los cores dentro de cada CPU asignando identificador y guardando espacio para los hilos
        {
            machine.cpu_array[i].core_array[j].core_id = core_id_aux;
            machine.cpu_array[i].core_array[j].hilos = (HwThread_t*)malloc(num_processes*sizeof(HwThread_t));

            core_id_aux++;
            for (int h = 0; h < num_processes; h++)
            {
               machine.cpu_array[i].core_array[j].hilos[h] = hilo_vacio;
            }
        }   
    }
    
    //En este punto la máquina está llena de hilos vacíos (en realidad todos apuntan al mismo)
    return 1;
}
int crear_hilos(int* frecuencias)
{
    //id[0] clock
    //id[1] dispacher
    //id[2] PGenerator
    //id[3] timer process generator
    //id[4] timer scheduler

    id = (pthread_t*)malloc((3+num_timers)*sizeof(pthread_t));

    int aux=0;
    
    pthread_create(&id[0],NULL,clock_function,&aux);
    pthread_create(&id[1],NULL,schedule_dispacher_function,&aux);
    pthread_create(&id[2],NULL,loader_function,&aux);
    
    for (int i = 0; i < num_timers; i++)
    {
        int *arg = malloc(sizeof(*arg));  //Aquí se guarda espacio para el indice
        if ( arg == NULL ) {
            printf("No se ha podido guardar espacio para arg\n");  
            exit(EXIT_FAILURE);
        }

        *arg = i;
        pthread_create(&id[3+i],NULL,timer_function,arg);
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
int inicializar_control(){

    pthread_mutex_init(&mtx,NULL);
    pthread_cond_init(&cond,NULL);
    pthread_cond_init(&cnd_br,NULL);
    pthread_mutex_init(&mtxpGenerator,NULL);
    pthread_cond_init(&condpGenerator,NULL);
    pthread_cond_init(&cnd_brpGenerator,NULL);
    pthread_mutex_init(&mtxScheduler,NULL);
    pthread_cond_init(&condScheduler,NULL);
    pthread_cond_init(&cnd_brScheduler,NULL);
    return (1);
}

int main(int argc, char *argv[]) {
   if (argc!=6) {
        printf("Error en argumentos, formato es: \n num_CPU, num_cores, num_hilos, f_pGenerator, f_Scheduler \n");
        exit(-1);
    }
    num_CPUs = atoi(argv[1]);
    num_cores = atoi(argv[2]);
    num_processes = atoi(argv[3]);

    num_timers = 2;
    frecuencias = (int*)malloc((num_timers)*sizeof(int));
    for (int j = 0; j < num_timers; j++)
    {
        frecuencias[j] = atoi(argv[4+j]);
    }
    
    inicializar_hardware();
    inicializar_control();
    crear_hilos(frecuencias);

    eliminar_hilos();
    return 0;
}
