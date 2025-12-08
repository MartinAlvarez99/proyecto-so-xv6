#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void cpu_waster(int k) {
  int i, j;
  for(i = 0; i < k; i++) {
    for(j = 0; j < 1000000; j++) {
      asm("nop"); 
    }
  }
}

int main(int argc, char *argv[]) {
  int pid;
  int start_time, end_time;

  printf("Iniciando prueba de planificador con prioridades...\n");

  for (int i = 0; i < 3; i++) {
    pid = fork();
    
    if (pid < 0) {
      printf("Error: fork fallo\n");
      exit(1);
    }

    if (pid == 0) {
      // --- CODIGO DEL HIJO ---
      if (i == 2) {
          printf("Hijo #%d (PID %d) iniciando con ALTA PRIORIDAD (20)\n", i, getpid());
          set_priority(20); 
      } else {
          printf("Hijo #%d (PID %d) iniciando con prioridad normal (10)\n", i, getpid());
          set_priority(10);
      }

      sleep(10); // Esperar 10 ticks para que todos los hijos nazcan

      start_time = uptime();
      cpu_waster(50000); // Simular carga de CPU
      end_time = uptime();

      printf(">>> Hijo #%d (PID %d) TERMINO. Tiempo total: %d ticks\n", i, getpid(), end_time - start_time);
      exit(0);
    }
    // El padre continua el bucle para crear al siguiente hijo...
  }

  // El padre espera a los 3 hijos
  for (int i = 0; i < 3; i++) {
    wait(0);
  }
  
  printf("Test finalizado exitosamente.\n");
  exit(0);
}