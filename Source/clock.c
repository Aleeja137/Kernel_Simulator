#include <pthread.h>
#include <stdio.h>
#include "clock.h"
#include "globals.h"

 void* clock_func_prueba(void * argument){
    printf("I am the clock\n");
    pthread_exit(NULL);
}


void* clock_function(void * argument){
    done_count = 0;
    while (1)
    {
        pthread_mutex_lock(&mtx);
        while (done_count<num_timers)
        {
            pthread_cond_wait(&cond,&mtx);
            //esperar que uno a uno se despierten los timers
        }
        reducirTtl(NULL);
        done_count=0;
        pthread_cond_broadcast(&cnd_br);
        pthread_mutex_unlock(&mtx);
    }
}
int internal_count = 0; //variable para que no reduzca el ttl cada ciclo de reloj
void* reducirTtl(void* arg){
    if(internal_count==1000000)
    {
        //Recorro los CPUs, cores e hilos reduciendo el ttl
        for (int i = 0; i < num_CPUs; i++)
        {
            for (int j = 0; j < num_cores; j++)
            {
                for (int k = 0; k < num_processes; k++)
                {
                    if (machine.cpu_array[i].core_array[j].process_array[k].asigned_core_id!=-1)
                    {
                        machine.cpu_array[i].core_array[j].process_array[k].ttl+=-1;
                        printf("Reducida la vida de %d a %d\n",
                            machine.cpu_array[i].core_array[j].process_array[k].pid,
                            machine.cpu_array[i].core_array[j].process_array[k].ttl);
                        //Si el ttl es cero, es decir, el proceso muere, no hace falta eliminarlo, 
                        //solo poner a -1 el int de asignación y el mismo scheduler ya lo tiene en cuenta como libre
                        if (machine.cpu_array[i].core_array[j].process_array[k].ttl == 0)
                        {
                            printf("Eliminado proceso %d \n",machine.cpu_array[i].core_array[j].process_array[k].pid);
                            machine.cpu_array[i].core_array[j].process_array[k].asigned_core_id = -1;
                        }            
                    }            
                }        
            }        
        }
        internal_count=0;
    } else {
        internal_count++;
    }
       
}