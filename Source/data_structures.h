// Estructuras de datos para el proyecto
#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

//------------------------------------//
// Estructuras para lista de procesos //
//------------------------------------//
typedef struct mm
{
    unsigned int code;  //Puntero a la dirección virtual de comienzo del segmento de código
    unsigned int data;  //Puntero a la dirección virtual de comienzo del segmento de datos
    unsigned int* pgb;  //Puntero a la dirección física de la correspondiente tabla de páginas, se usa* ya que no es un valor sino un array
    int num_frames;   //Cantidad de frames que necesita el proceso (NECESARIO?? - SI, para actualizar_TLB() )

} mm_t;

typedef struct process_info  //PCB
{
    //int ttl;              //Time to live
    //int asigned_core_id;  //Identificador del core al que se le ha asignado el proceso, -1 si no está asignado
    int pid;                //Identificador de proceso
    int status;             //-1 si acabado, 0 en espera, 1 en ejecución
    mm_t mm;                //Estructura que contiene info adicional

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

//----------------------------------------------------//
// Estructuras para lista de frames en memoria física //
//----------------------------------------------------//
typedef struct frame
{
    unsigned int address;
    struct frame* next;

} frame_t;

typedef struct frames_list
{
    int free_frames;
    frame_t* first;
    frame_t* last;

} frames_list;


//-----------------------------------------//
// Estructuras para representar la máquina //
//-----------------------------------------//

typedef struct TLB //Translation Lookaside Buffer, une la memoria física con la virtual, siorve como caché de la MMU
{
    unsigned int physical;
    unsigned int virtual;
    int valid;  //-1 si no, 1 si si
    int viejo;

} TLB_t;

typedef struct MMU //Memory Management Unit
{
    TLB_t TLB[4]; //Arbitrariamente he elegido 4

} MMU_t;

typedef struct HwThread
{
    process_info_t proceso;     //Tiene el proceso que el hilo está trabajando
    unsigned int* PTBR;         //Apunta a la dirección en memoria física donde empieza la tabla de páginas
    unsigned int PC;           //Apunta a la instrucción actual que se ejecuta del proceso en el PCB
    unsigned int IR;           //Va leyendo la instrucción actual, decodificada
    MMU_t MMU;                  //Memory Management Unit
    int registros[16];          //Registros generales

} HwThread_t;

typedef struct core //Core, lista de hilos
{
    int core_id;
    HwThread_t* hilos;
} core_t;

typedef struct cpu //CPU, lista de cores
{
    int cpu_id;
    core_t* core_array;
} cpu_t;
typedef struct machine  //Máquina principal, lista de CPUs
{
    //falta string con nombre de la máquina ??
    cpu_t* cpu_array;
} machine_t;

#endif //DATA_STRUCTURES_H