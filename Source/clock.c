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
        done_count=0;
        pthread_cond_broadcast(&cnd_br);
        pthread_mutex_unlock(&mtx);
    }

    /*done_count = 0;
    while (1)
    {
        printf("Barrera 1");
        pthread_mutex_lock(&mtx);
        while (done_count<num_timers)
        {
            pthread_cond_wait(&cond,&mtx);
            printf("Barrera 2");
            //esperar que uno a uno se despierten los timers
        }
        printf("Barrera 3");
        done_count=0;
        process_node_t* link = lista_procesos.first;printf("Barrera ");
        while (link->next!=NULL){
            printf("Barrera 4");
           //Recorro los CPUs, cores e hilos reduciendo el ttl
            for (int i = 0; i < num_CPUs; i++)
            {
                printf("Barrera 5");
                for (int j = 0; j < num_cores; j++)
                {
                    printf("Barrera 6");
                    for (int k = 0; k < num_processes; k++)
                    {
                        printf("Barrera 7");
                        if (machine.cpu_array[i].core_array[j].process_array[k].asigned_core_id!=-1)
                        {
                            printf("Barrera 8");
                            machine.cpu_array[i].core_array[j].process_array[k].ttl+=-1;
                            //Si el ttl es cero, es decir, el proceso muere, no hace falta eliminarlo, 
                            //solo poner a -1 el int de asignación y el mismo scheduler ya lo tiene en cuenta como libre
                            if (machine.cpu_array[i].core_array[j].process_array[k].ttl == 0)
                            {
                                printf("Barrera 9");
                                machine.cpu_array[i].core_array[j].process_array[k].asigned_core_id = -1;
                            }
                            
                        }
                        
                    }
                    
                }
                
            }
        }
        printf("Barrera 10");
        pthread_cond_broadcast(&cnd_br);
        pthread_mutex_unlock(&mtx);
    }*/
}