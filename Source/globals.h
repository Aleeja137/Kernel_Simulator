extern int num_CPUs; //Número de procesadores de la máquina
extern int num_cores; //Número de cores por CPU
extern int num_processes; //Número de hilos por core
//extern machine_t machine; //Machine object
int* frecuencias;  //Array que guarda las frecuencias de los timers
extern int num_timers; //Número de timers
extern int done_count;//variable que dice cuántos timers han recibido el pulso del clock 
extern pthread_t* id; //Array que guarda los ID de los threads para el clock, timer, dispacher y PGenerator

extern pthread_mutex_t mtx; //variable lock para el clock/timer
extern pthread_cond_t cond; //variable de espera condicional para clock/timer
extern pthread_cond_t cnd_br; //variable de condición de broadcast para clock/timer
