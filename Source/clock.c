#include <pthread.h>
#include <stdio.h>
#include "clock.h"

 void* clock_func_prueba(void * argument){
    printf("I am the clock boy \n");
    pthread_exit(NULL);
}