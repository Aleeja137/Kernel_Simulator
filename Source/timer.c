#include <pthread.h>
#include <stdio.h>
#include <pthread.h>

#include "timer.h"
#include "globals.h"

 void* timer_func_prueba(void * argument){
    printf("I am the timer boy \n");
    pthread_exit(NULL);
}
void* timer_function(int indice_frec)
{
    int cuenta = 0;
    mutex_lock(&mutex_clock);
    while(1)
    {
        done_count++;
        cuenta++;
        if (cuenta==frecuencias[indice_frec])
        {
           printf("Me he despertado con frecuencia %d",frecuencias[indice_frec]);
           cuenta = 0;
        }
        cond_signal(&cond_clock);
        cond_wait(&cond_clock_broadcast,&mutex_clock);
    }
}