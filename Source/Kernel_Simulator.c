#include <stdio.h>
#include <stdlib.h>

#include "data_structures.h"
#include "globals.h"

//Variables globales
int num_CPUs;
int num_cores;
int num_processes;

machine_t machine;

//COSAS PARA HACER EN GENERAL
//La comunicación entre hilos mediante mutex_t y cond_t

//COSAS PARA HACER AQUI
//Inicializar estructuras (los de data_structures con los parámetros recibidos)
//Llamar a los pthreads y asignar un papel a cada
int main(int argc, char *argv[]) {
    num_CPUs = (argv[1]-'0');
    num_cores = (argv[2]-'0');
    num_processes = (argv[3]-'0');

    //inicializar la máquina
    int cpu_id_aux=0;
    int core_id_aux=0;
    machine.cpu_array = (cpu_t*)malloc(num_CPUs*sizeof(cpu_t)); //Se guarda espacio para las CPUs
    for (int i = 0; i < num_CPUs; i++)  //Recorre el array de CPUs asignando un identificador y guardando espacio para los cores
    {
        machine.cpu_array[i].cpu_id = cpu_id_aux;
        machine.cpu_array[i].core_array = (core_t*)malloc(num_cores*sizeof(core_t));

        for (int j = 0; j < num_cores; j++)  //Recorre los cores dentro de cada CPU asignando identificador y guardando espacio para los hilos
        {
            machine.cpu_array[i].core_array[j].core_id = core_id_aux;
            machine.cpu_array[i].core_array[j].process_array = (process_info_t*)malloc(num_processes*sizeof(process_info_t));           
            core_id_aux++;
        }
        cpu_id_aux++; 
    }
}
