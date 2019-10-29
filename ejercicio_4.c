#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

void generarHijos(int numero);
void printChildrenIDs();
void signalChildHandler(int signal);
void signalWaiting(int signal);
void changeDirection(int signal);
void exitProgram(int signal);
void fillArray();

int valor[10];
int parentID = 0;
int order = 0;

int main () {

  int hijos;
  fillArray();

  signal(SIGUSR1, signalWaiting);
  signal(SIGTSTP, changeDirection);
  signal(SIGUSR2, signalChildHandler);

  printf("Para cambiar de direccion: Control - Z, Para terminar procesos hijos: Control - C\n");
  printf("Cuantos hijos desea generar? ");
  scanf("%d", &hijos);

  generarHijos(hijos);

  wait(NULL);

  return 0;
}

void generarHijos(int hijos) {
  parentID = getpid();
  printf("\nESte es el valor del papa papa: %d\n", parentID);

  for (int i = 0; i < hijos; i++) {
    if (parentID == getpid()) {
      int proceso = fork();
      if (proceso != 0 && proceso != parentID) {
          valor[i] = proceso;
          printf("Proceso hijo #%d: %d\n", i + 1 , valor[i]);
          signal(SIGTERM, signalChildHandler);
      }

    }
  }

  if (getpid() != parentID) {
    while (1);
  }
  else {
    signal(SIGINT, exitProgram);
    while (1) {
      printChildrenIDs();

    }
  }

}

void printChildrenIDs () {
  if (order == 0) {
    printf("\n");
    for (int i = 0; i < (sizeof(valor)/sizeof(valor[0])); i++) {
      printf("Valor: %d\n", valor[i]);
      if (order == 1 || valor[i] == -1) break;
      fprintf(stderr,"PID proceso hijo #%d ", i + 1 );
      kill(valor[i], SIGUSR2);
      pause();
    }
  }
  else {
    printf("\n");
    for (int i = (sizeof(valor)/sizeof(valor[0])) - 1; i >= 0 ; i--) {
      if (order == 0) break;
      if (valor[i] != -1) {
        fprintf(stderr,"PID proceso hijo #%d ",  i + 1);
        kill(valor[i], SIGUSR2);
        pause();
      }

    }
  }
}

void signalChildHandler (int signalint) {
  fprintf(stderr,"%d\n", getpid());
  sleep(1);
  kill(parentID, SIGUSR1);
}

void signalWaiting (int signal) {}
void changeDirection (int signal) {
  order = !order;
}

void exitProgram (int signal) {
  printf("\n");
  for (int i = 0;  i < (sizeof(valor)/sizeof(valor[0])); i++) {
    kill(valor[i], SIGINT);
    printf("Proceso eliminado\n");
  }

  exit(0);
}

void fillArray () {
  for (int i = 0;  i < (sizeof(valor)/sizeof(valor[0])); i++) {
    valor[i] = -1;
  }
}
  //printf("Se esta ejecutando esta linea. Signal: %d\n", signal);
