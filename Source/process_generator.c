#include <pthread.h>
#include <stdio.h>
#include "process_generator.h"

 void* process_generator_func_prueba(void * argument){
    printf("I am the PGenerator boy \n");
    pthread_exit(NULL);
}