#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "data_structures.h"
#include "process_generator.h"

 void* process_generator_func_prueba(void * argument){
    printf("I am the PGenerator\n");
    pthread_exit(NULL);
}

void* process_generator_function(void* args){
    for (int i = 0; i < 50; i++)
    {
        //Crear nuevo proceso
        process_node_t* nuevo_process_node = (process_node_t*)malloc(sizeof(process_node_t));  
        process_info_t nuevo_process_info;
        nuevo_process_info.pid = i;
        nuevo_process_info.ttl = ((rand() % 40)+10);
        nuevo_process_info.asigned_core_id = NULL;
        nuevo_process_node->me = nuevo_process_info;
        nuevo_process_node->next = NULL;

        //Meterlo en la cola
        if (lista_procesos.first==NULL){  //La lista está vacía
            lista_procesos.first = nuevo_process_node;
            lista_procesos.last = nuevo_process_node;
        } else {                          //Se actualiza el último
            lista_procesos.last->next = nuevo_process_node;
            lista_procesos.last = nuevo_process_node;
        }
        //printf("Creado el proceso %d, con ttl %d \n",lista_procesos.last->me.pid,lista_procesos.last->me.ttl);
    }
    
}