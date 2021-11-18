#include <pthread.h>
#include <stdio.h>
#include "clock.h"
#include "globals.h"

 void* clock_func_prueba(void * argument){
    printf("I am the clock boy \n");
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
}