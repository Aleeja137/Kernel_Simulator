#include <pthread.h>
#include <stdio.h>
#include "clock.h"

 void* clock_func_prueba(void * argument){
    printf("I am the clock boy \n");
    pthread_exit(NULL);
<<<<<<< HEAD
}
void* clock_function ()
{
    done_count = 0;
    
    while(1)
    {
        mutex_lock(&mutex_clock);
        while (done_count < num_timers)
        {
            cond_wait(condiciones[cond_clock],&mutex_clock);
            done_count++;
        }
        done_count = 0;
        cond_broadcast(condiciones[cond_clock_broadcast]);
        mutex_unlock(&mutex_clock);
    }
=======
>>>>>>> parent of 5a7dd54 (Updates)
}