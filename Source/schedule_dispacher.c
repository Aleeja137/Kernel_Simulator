#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include "globals.h"
#include "schedule_dispacher.h"

 void* schedule_dispacher_func_prueba(void * argument){
    printf("I am the scheduler\n");
    pthread_exit(NULL);
}

 void* schedule_dispacher_function(void * argument){
    bool asignado;
    while (1)
    {
        //Si hay procesos en la cola
        if (lista_procesos.first!=NULL){
            asignado=false;
            //Recorrer los cores e hilos asignando el primer proceso en la cola al primer hueco libre
            for (int i = 0; i < num_CPUs && asignado==false; i++){
                for (int j = 0; j < num_cores && asignado==false; j++){
                    for (int k = 0; k < num_processes && asignado==false; k++){
                        if (machine.cpu_array[i].core_array[j].process_array[k].asigned_core_id==NULL)
                        {
                            machine.cpu_array[i].core_array[j].process_array[k] = lista_procesos.first->me;
                            machine.cpu_array[i].core_array[j].process_array[k].asigned_core_id = machine.cpu_array[i].core_array[j].core_id;                                asignado=true;
                            asignado=true;
                        }    
                    }    
                }    
            }    
        }
    }
}
