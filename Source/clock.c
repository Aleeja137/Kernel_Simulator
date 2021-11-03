#include <pthread.h>
#include <stdio.h>
#include <pthread.h>

#include "globals.h"
#include "clock.h"

 void* clock_func_prueba(void * argument){
    printf("I am the clock boy \n");
    pthread_exit(NULL);
}
void* clock_function ()
{
    done_count = 0;
    
    while(1)
    {
        mutex_lock(&mutex_clock);
        while (done_count < num_timers)
        {
            cond_wait(&cond_clock,&mutex_clock);
            done_count++;
        }
        done_count = 0;
        cond_broadcast(&cond_clock_broadcast);
        mutex_unlock(&mutex_clock);
    }
}