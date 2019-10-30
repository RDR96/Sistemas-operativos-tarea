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
int counter = 0;

int main () {

  int hijos;
  fillArray();

  signal(SIGUSR1, signalWaiting);
  signal(SIGUSR2, signalChildHandler);
  signal(SIGTSTP, changeDirection);


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

  if (counter == (sizeof(valor)/sizeof(valor[0])))
    counter = 0;

  if (counter < 0)
    counter = (sizeof(valor)/sizeof(valor[0])) - 1;


  if (valor[counter] != -1) {
    fprintf(stderr,"PID proceso hijo #%d ", counter + 1 );
    kill(valor[counter], SIGUSR2);
    pause();
  }

  if (order == 0 )
    counter++;
  else
    counter--;

}

void signalChildHandler (int signalint) {
  fprintf(stderr,"%d\n", getpid());
  sleep(1);
  kill(parentID, SIGUSR1);
}

void signalWaiting (int signal) {}

void changeDirection (int signal) {
  if (getpid() == parentID)
    order = !order;
}

void exitProgram (int signal) {
  if (getpid() == parentID) {
    printf("\n");
    for (int i = 0;  i < (sizeof(valor)/sizeof(valor[0])); i++) {
      kill(valor[i], SIGINT);
      printf("Proceso eliminado\n");
    }

    exit(0);
  }

}

void fillArray () {
  for (int i = 0;  i < (sizeof(valor)/sizeof(valor[0])); i++) {
    valor[i] = -1;
  }
}
  //printf("Se esta ejecutando esta linea. Signal: %d\n", signal);
