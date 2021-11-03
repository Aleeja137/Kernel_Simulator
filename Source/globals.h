extern int num_CPUs; //Número de procesadores de la máquina
extern int num_cores; //Número de cores por CPU
extern int num_processes; //Número de hilos por core
//extern machine_t machine; //Machine object
extern int done_count;//variable que dice cuántos timers han recibido el pulso del clock 
extern int num_timers; //Número de timers
extern pthread_t* id; //Array que guarda los ID de los threads para el clock, timer, dispacher y PGenerator
extern pthread_cond_t cond_clock; //variable de condición para mutex entre clock y timer
extern pthread_cond_t cond_clock_broadcast; //variable condicional para avisar a los timers que sigan
extern pthread_mutex_t mutex_clock; //mutex para la sincronización entre clock y timer



