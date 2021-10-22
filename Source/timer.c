#include <pthread.h>
#include <stdio.h>
#include "timer.h"

 void* timer_func_prueba(void * argument){
    printf("I am the timer boy \n");
    pthread_exit(NULL);
}