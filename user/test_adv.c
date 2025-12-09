#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// ---------------- Simulación de espera sin sleep() ----------------
void sleep_fake(int ticks) {
  int start = uptime();
  while (uptime() - start < ticks) {
    ;  // busy wait → cede CPU por interrupciones del timer
  }
}

// ---------------- CPU-bound ----------------
void cpu_task(int loops) {
  volatile int i;
  for (i = 0; i < loops; i++) {
    asm("nop");
  }
}

// ---------------- I/O-bound ----------------
void io_task(int loops) {
  for (int i = 0; i < loops; i++) {
    sleep_fake(1);   // "cede" CPU constantemente
  }
}

// ---------------- MAIN ----------------
int main(int argc, char *argv[]) {

  printf("===== Test avanzado de scheduler (sin yield) =====\n");

  int pid;

  // ---------------- Lanzar 3 procesos CPU-bound ----------------
  for (int i = 0; i < 3; i++) {
    pid = fork();
    if (pid == 0) {
      int start = uptime();
      printf("[CPU-Bound] Proceso %d iniciado\n", getpid());

      cpu_task(120000000);   // mucha carga de CPU

      int end = uptime();
      printf("[CPU-Bound] Proceso %d terminó. Ticks = %d\n",
             getpid(), end - start);
      exit(0);
    }
  }

  // ---------------- Lanzar 3 procesos IO-bound ----------------
  for (int i = 0; i < 3; i++) {
    pid = fork();
    if (pid == 0) {
      int start = uptime();
      printf("[IO-Bound] Proceso %d iniciado\n", getpid());

      io_task(4000);   // cede CPU en cada iteración

      int end = uptime();
      printf("[IO-Bound] Proceso %d terminó. Ticks = %d\n",
             getpid(), end - start);
      exit(0);
    }
  }

  // Padre espera a todos
  for (int i = 0; i < 6; i++) {
    wait(0);
  }

  printf("===== Test finalizado =====\n");
  exit(0);
}
