// Estructuras de datos para el proyecto
#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

//------------------------------------//
// Estructuras para lista de procesos //
//------------------------------------//

typedef struct process_info  //PCB
{
    int pid;
    int ttl;
} process_info_t;

typedef struct process_node  //Nodo para la lista de PCBs
{
    struct process_node * next;
    process_info_t me;
} process_node_t;

typedef struct process_queue  //Lista de PCBs
{
    process_node_t* first;
    process_node_t* last;
} process_queue_t;

//-----------------------------------------//
// Estructuras para representar la máquina //
//-----------------------------------------//


typedef struct core //Core, lista de hilos
{
    int core_id;
    int* id_procesos;
} core_t;
typedef struct cpu //CPU, lista de cores
{
    int cpu_id;
    core_t* core_array;
} cpu_t;
typedef struct machine  //Máquina principal, lista de CPUs
{
    //falta string con nombre de la máquina
    cpu_t* cpu_array;
} machine_t;

#endif //DATA_STRUCTURES_H