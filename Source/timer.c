#include <pthread.h>
#include <stdio.h>
#include "timer.h"
#include "globals.h"

 void* timer_func_prueba(void * argument){
    printf("I am the timer\n");
    pthread_exit(NULL);
}

void* timer_function(void * indice_frec)
{
    printf("Me he despertado con frecuencia %d\n",frecuencias[*(int*)indice_frec]);
    int cuenta = 0;
    pthread_mutex_lock(&mtx);
    while(1){
        done_count++;
        //hacer cosas
        cuenta++;
        if (cuenta==frecuencias[*(int*)indice_frec]){
            printf("I woke up, Im timer %d \n",*((int*)indice_frec));
            cuenta=0;
        }
        pthread_cond_signal(&cond);
        pthread_cond_wait(&cnd_br,&mtx);
    }
}
