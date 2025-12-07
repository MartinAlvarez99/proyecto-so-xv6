#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Una función que gasta CPU tontamente
void cpu_waster(int k) {
  int i, j;
  // Hacemos muchas iteraciones para que se note el tiempo
  for(i = 0; i < k; i++) {
    for(j = 0; j < 1000000; j++) {
      // Operaciones matemáticas simples para consumir ciclos
      asm("nop"); 
    }
  }
}

int main(int argc, char *argv[]) {
  int pid;
  int start_time, end_time;

  // Vamos a lanzar 3 procesos hijos
  // En el xv6 original, todos deberían ser tratados igual.
  for (int i = 0; i < 3; i++) {
    pid = fork();
    
    if (pid < 0) {
      printf("Error al hacer fork\n");
      exit(1);
    }

    if (pid == 0) {
      // Este es el proceso HIJO
      start_time = uptime(); // Hora de inicio (en ticks)
      printf("Proceso %d iniciado (Hijo #%d)\n", getpid(), i);
      
      cpu_waster(500000); 
      
      end_time = uptime(); // Hora de fin
      printf("Proceso %d termino. Tiempo: %d ticks\n", getpid(), end_time - start_time);
      exit(0); // El hijo muere aquí
    }
  }

  // El proceso PADRE espera a que todos los hijos terminen
  for (int i = 0; i < 3; i++) {
    wait(0);
  }
  
  printf("Test finalizado.\n");
  exit(0);
}