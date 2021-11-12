extern int num_CPUs; //Número de procesadores de la máquina
extern int num_cores; //Número de cores por CPU
extern int num_processes; //Número de hilos por core
//extern machine_t machine; //Machine object
int* frecuencias;  //Array que guarda las frecuencias de los timers
extern int num_timers; //Número de timers
//extern int done_count;//variable que dice cuántos timers han recibido el pulso del clock 
extern pthread_t* id; //Array que guarda los ID de los threads para el clock, timer, dispacher y PGenerator
//extern pthread_cond_t* condiciones;//Array que guarda las variables condicionales
//extern int cond_clock; //indice dentro de 'condiciones' para mutex entre clock y timer
//extern int cond_clock_broadcast; //indice dentro de 'condiciones' para avisar a los timers que sigan
//extern pthread_mutex_t mutex_clock; //mutex para la sincronización entre clock y timer