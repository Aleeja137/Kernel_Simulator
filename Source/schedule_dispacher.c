#include <pthread.h>
#include <stdio.h>
#include "schedule_dispacher.h"

 void* schedule_dispacher_func_prueba(void * argument){
    printf("I am the scheduler boy \n");
    pthread_exit(NULL);
}