#include "data_structures.h"
extern int num_CPUs; //Número de procesadores de la máquina
extern int num_cores; //Número de cores por CPU
extern int num_processes; //Número de hilos por core

extern machine_t machine; //Machine object
extern process_queue_t lista_procesos; //Lista con todos los procesos

int* frecuencias;  //Array que guarda las frecuencias de los timers
extern int num_timers; //Número de timers

extern pthread_t* id; //Array que guarda los ID de los threads para el clock, timer, dispacher y PGenerator

extern int done_count;//variable que dice cuántos timers han recibido el pulso del clock 
extern pthread_mutex_t mtx; //variable lock para el clock/timer
extern pthread_cond_t cond; //variable de espera condicional para clock/timer
extern pthread_cond_t cnd_br; //variable de condición de broadcast para clock/timer

extern int done_countScheduler;//variable que dice si el Scheduler ha recibido el pulso del timer
extern pthread_mutex_t mtxScheduler; //variable lock para el timer/scheduler
extern pthread_cond_t condScheduler; //variable de espera condicional para timer/scheduler
extern pthread_cond_t cnd_brScheduler; //variable de condición de broadcast para timer/scheduler

extern int done_countpGenerator;//variable que dice si el pGenerator ha recibido el pulso del timer
extern pthread_mutex_t mtxpGenerator; //variable lock para el timer/pGenerator
extern pthread_cond_t condpGenerator; //variable de espera condicional para timer/pGenerator
extern pthread_cond_t cnd_brpGenerator; //variable de condición de broadcast para timer/pGenerator