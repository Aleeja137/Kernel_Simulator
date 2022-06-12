#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include "globals.h"
#include "schedule_dispacher.h"

 void* schedule_dispacher_func_prueba(void * argument){
    printf("I am the scheduler\n");
    pthread_exit(NULL);
}

 void* schedule_dispacher_function(void * argument){
    bool asignado;
    pthread_mutex_lock(&mtxScheduler);
    while (1)
    {
        done_countScheduler++;
        // CONTROL : printf("Llamado al dispatcher desde el timer\n");
        //Si hay procesos en la cola
        if (lista_procesos.first!=NULL){
            asignado=false;
            //Recorrer los cores e hilos asignando el primer proceso en la cola al primer hueco libre
            for (int i = 0; i < num_CPUs && asignado==false; i++){
                for (int j = 0; j < num_cores && asignado==false; j++){
                    for (int k = 0; k < num_processes && asignado==false; k++){
                        if (machine.cpu_array[i].core_array[j].hilos[k].proceso.status == -1) //Significa que el proceso ha acabado, es decir, el hilo está libre
                        {
                            HwThread_t new_thread;

                            new_thread.proceso = lista_procesos.first->me;
                            new_thread.PC = lista_procesos.first->me.mm.data;
                            new_thread.PTBR = lista_procesos.first->me.mm.pgb;
                            new_thread.proceso.status = 1;
                            for (int p = 0; p < 4; p++)
                            {
                                new_thread.MMU.TLB[p].valid = -1;
                            }

                            machine.cpu_array[i].core_array[j].hilos[k] = new_thread;
                            asignado=true;

                            // Si solo hay un proceso en la lista, el último va a ser null
                            if (lista_procesos.last == lista_procesos.first)
                            {
                                lista_procesos.last = NULL;
                            }
                            // En cualquier caso, el rimero debe apuntar al siguiente (si es el único se queda en null, como debería)
                            lista_procesos.first = lista_procesos.first->next;
                            
                            printf("Asignado proceso %d al hilo %i del core %i de la cpu %i, teniendo data en %d y text en %d\n",
                                machine.cpu_array[i].core_array[j].hilos[k].proceso.pid,
                                k,
                                machine.cpu_array[i].core_array[j].core_id,
                                machine.cpu_array[i].cpu_id,
                                machine.cpu_array[i].core_array[j].hilos[k].proceso.mm.code,
                                machine.cpu_array[i].core_array[j].hilos[k].proceso.mm.data);
                        }    
                    }    
                }    
            }    
        } else {
            printf("No hay procesos en espera \n");
        }
        pthread_cond_signal(&condScheduler);
        pthread_cond_wait(&cnd_brScheduler,&mtxScheduler);
    }
}
