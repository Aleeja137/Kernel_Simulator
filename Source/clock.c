#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "clock.h"
#include "globals.h"
#include "data_structures.h"

 void* clock_func_prueba(void * argument){
    printf("I am the clock\n");
    pthread_exit(NULL);
}


//Sustituye la primera traducción no válida del TLB con la traducción encontrada en la pgb
//Devuelve -1 si no está en el pgb
int actualizar_TLB(unsigned int dir,HwThread_t hilo){ 
    int res = -1;
    //Busca el primer TLB no válido, si todos son válidos sustituye el último arbitrariamente
    int done = -1;
    int index = 0;
    while (done < 0 && index < 4){
        if (hilo.MMU.TLB[index].valid == -1){
            done = 1;
        } else {
            index++;
        }
    }

    done = -1;
    int index2 = 0;
    while (done < 0 && index2 < hilo.proceso.mm.num_frames)
    {
        unsigned int mask;
        mask = (1 << 5) - 1;
        //Si los primeros 19 bits de los 24 de la dirección virtual coinciden con los primeros 19 del puntero del frame, significa que está en ese frame
        if ((dir & mask) == (hilo.proceso.mm.pgb[index2] & mask))
        {
            done = 1;
            hilo.MMU.TLB[index].physical = hilo.proceso.mm.pgb[index2];
            hilo.MMU.TLB[index].virtual = (dir & mask);
            hilo.MMU.TLB[index].valid = 1;
            res = index2;
        } else {
            index2++;
        }
        return res;
    }
}


//Función que busca si la memoria virtual está en la TLB
//Si no está, actualiza la TLB 
//Devuelve el índice del TLB, devuelve -1 si la dirección no corresponde al pgb
int esta_en_TLB(unsigned int dir,HwThread_t hilo){ 
    int res = -1;
    unsigned int mask;
    mask = (1 << 5) - 1;
    for (int i = 0; i < 4; i++)
    {
        //Si el TLB es válido y la dirección virtual está en la misma página
        if (hilo.MMU.TLB[i].valid == 1 && ((hilo.MMU.TLB[i].virtual & mask) == (dir & mask)))
        {
            res = i;
        }
        
    }
    if (res < 0)
    {
        res = actualizar_TLB(dir,hilo);
    }
    
    return res; 
}


//Busca la dirección fisica que coincida con la dirección virtual en la TLB que contiene la info
//No tiene mucho sentido, ya que como estamos manualmente guardando los datos en una memoria
//vacía del mismo tamaño que la memoria virtual, las direcciones son las mismas, pero es una función que debería estar
unsigned int buscar_en_TLB(int index, unsigned int dir, HwThread_t hilo){
    if (index == -1)
    {
        return 0x00;
    }
    
    unsigned int res=0x0;
    for (int i = 0; i < 256; i++)
    {
        if (hilo.MMU.TLB[index].physical+i == dir)
        {
            res = hilo.MMU.TLB[index].physical+i;
        }
        
    }
    return res;
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
        reducirTtl(NULL);
        done_count=0;
        pthread_cond_broadcast(&cnd_br);
        pthread_mutex_unlock(&mtx);
    }
}

void* reducirTtl(void* arg){
    //Recorro los CPUs, cores e hilos haciendo la primera operación de donde aputa el PC
    for (int i = 0; i < num_CPUs; i++){
        for (int j = 0; j < num_cores; j++){
            for (int k = 0; k < num_processes; k++){
                HwThread_t hilo = machine.cpu_array[i].core_array[j].hilos[k];

                if(hilo.proceso.status == 1) //Significa que no está acabado y que está asignado
                {
                    unsigned int direccion_fisica;
                    unsigned int direccion_virtual;
                    int res;
                    direccion_virtual = hilo.PC;
                    res = esta_en_TLB(direccion_virtual,hilo);
                    if (res < 0)
                    {
                        printf("Invalid memory access\n");
                    } else {
                        direccion_fisica = buscar_en_TLB(res,direccion_virtual,hilo);
                        hilo.IR = memoria_fisica[direccion_fisica];
                        // printf("Hilo %i del core %i de la cpu %i\n la instrucción es: %d\n ",
                        //         machine.cpu_array[i].core_array[j].hilos[k].proceso.pid,
                        //         machine.cpu_array[i].core_array[j].core_id,
                        //         machine.cpu_array[i].cpu_id,
                        //         hilo.IR);
                        unsigned int operacion = (hilo.IR >> 28) & 0xF;
                        unsigned int r0, r1, r2;

                        switch(operacion)
                        {
                            case 0x0:   //ld rd, address
                            {
                                // printf("Entré en el load del hilo %i del core %i de la cpu %i\n ",
                                //         machine.cpu_array[i].core_array[j].hilos[k].proceso.pid,
                                //         machine.cpu_array[i].core_array[j].core_id,
                                //         machine.cpu_array[i].cpu_id);

                                r0 = (hilo.IR >> 24) & 0x0F;
                                direccion_virtual = hilo.IR & 0x0FFF;
                                //Busca la dirección en memoria física
                                direccion_fisica = buscar_en_TLB(esta_en_TLB(direccion_virtual,hilo),direccion_virtual,hilo);
                                if(direccion_fisica!=0x0){
                                    hilo.registros[r0] = memoria_fisica[direccion_fisica];
                                    printf("Guardado el valor %d en el registro %i\n",memoria_fisica[direccion_fisica],r0);
                                }
                                break;
                            }
                            case 0x1:   //st rf, address
                            {
                                // printf("Entré en el store del hilo %i del core %i de la cpu %i\n ",
                                //         machine.cpu_array[i].core_array[j].hilos[k].proceso.pid,
                                //         machine.cpu_array[i].core_array[j].core_id,
                                //         machine.cpu_array[i].cpu_id);

                                r0 = (hilo.IR >> 24) & 0x0F;
                                direccion_virtual = hilo.IR & 0x0FFF;
                                //Busca la dirección en memoria física
                                direccion_fisica = buscar_en_TLB(esta_en_TLB(direccion_virtual,hilo),direccion_virtual,hilo);
                                if(direccion_fisica!=0x0){
                                    memoria_fisica[direccion_fisica] = hilo.registros[r0];
                                    printf("Guardado el valor del registro %i en la dirección de memoria %d\n",r0, direccion_fisica);
                                }
                                break;
                            }
                            case 0x2:   // add rd, r1, r2
                            {
                                // printf("Entré en el load del hilo %i del core %i de la cpu %i\n ",
                                //         machine.cpu_array[i].core_array[j].hilos[k].proceso.pid,
                                //         machine.cpu_array[i].core_array[j].core_id,
                                //         machine.cpu_array[i].cpu_id);

                                r0 = (hilo.IR >> 24) & 0x0F;
                                r1 = (hilo.IR >> 20) & 0x0F;
                                r2 = (hilo.IR >> 16) & 0x0F;
                                hilo.registros[r0] = hilo.registros[r1] + hilo.registros[r2];
                                printf("Suma realizada de los registros %i y %i dejada en el registro %i con valor %d",r1,r2,r0,hilo.registros[r0]);
                                break;
                            }
                            case 0xF:   //exit
                            {
                                // printf("Entré en el exit del hilo %i del core %i de la cpu %i\n ",
                                //         machine.cpu_array[i].core_array[j].hilos[k].proceso.pid,
                                //         machine.cpu_array[i].core_array[j].core_id,
                                //         machine.cpu_array[i].cpu_id);

                                //Matar el proceso
                                hilo.proceso.status = -1;
                                
                                //Liberar espacio en memoria
                                for (int i = 0; i < hilo.proceso.mm.num_frames; i++)
                                {
                                    frame_t* new_frame = (frame_t*)malloc(sizeof(frame_t));
                                    new_frame->address = hilo.proceso.mm.pgb[i];
                                    new_frame->next = NULL;

                                    if (lista_frames.first == NULL)
                                    {
                                        lista_frames.first = new_frame;
                                        lista_frames.free_frames = 1;
                                    } else {
                                        lista_frames.last->next = new_frame;
                                        lista_frames.free_frames++;
                                    }
                                    lista_frames.last = new_frame;
                                }

                                //Liberar cache
                                for (int i = 0; i < 4; i++)
                                {
                                    hilo.MMU.TLB[i].valid = -1;
                                }

                                printf("Program exit\n");
                                break;
                                
                                
                            }
                            default:
                                break;

                        }


                    }
                    hilo.PC += 32;
                    
                }

            }        
        }        
    }

}
