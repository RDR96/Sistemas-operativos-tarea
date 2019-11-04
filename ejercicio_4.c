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
void closeChildProccess(int signal);
void fillArray();

int valor[30];
int parentID = 0;
int order = 0;
int counter = 0;
int valorP = 1;

sigset_t sig;
int s;
int sigVal;

int main () {

  fillArray();
  signal(SIGUSR1, signalWaiting);
  signal(SIGUSR2, signalChildHandler);
  signal(SIGTSTP, changeDirection);

  sigemptyset(&sig);

  sigaddset(&sig, SIGQUIT);
  s = pthread_sigmask(SIG_BLOCK, &sig, NULL);

  printf("Para cambiar de direccion: Control - Z, Para terminar procesos hijos: Control - C\n");
  printf("Cuantos hijos desea generar? ");

  char numeros[100];
  char* ptr;
  fgets(numeros, 100, stdin);

  long int hijos = strtol(numeros, &ptr, 10);
  if (hijos == 0) {
    printf("No se ingreso un numero\n");
    exit(0);
  }

  printf("%lu\n", hijos);


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

    while (valorP) {
      printChildrenIDs();
    }
  }


}

void printChildrenIDs () {

  if (counter == (sizeof(valor)/sizeof(valor[0])))
    counter = 0;

  if (counter < 0)
    counter = (sizeof(valor)/sizeof(valor[0])) - 1;


  if (valor[counter] != -1 && valorP == 1) {
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
  if (getpid() != parentID) {
    fprintf(stderr,"%d\n", getpid());
    sleep(1);
    kill(parentID, SIGUSR1);
  }
}

void signalWaiting (int signal) { }

void changeDirection (int signal) {
  if (getpid() == parentID)
    order = !order;
}

void exitProgram (int signal) {
  if (getpid() == parentID) {
    valorP = 0;
    printf("\n");
    for (int i = 0;  i < (sizeof(valor)/sizeof(valor[0])) ; i++) {
      if (valor[i] != -1) {
        printf("Proceso eliminado: id %d\n", valor[i]);
        kill(valor[i], SIGTERM);
        sleep(1);
      }
    }
    exit(0);
    printf("SAlio aqui, aun no se arruina");
  }
}

void closeChildProccess (int signal) {
  fprintf(stderr,"Proceso eliminado: %d, %d\n", getpid(), signal);
  if (parentID != getpid()) {
    printf("Proceso eliminado: %d, %d\n", getpid(), signal);
    kill(parentID, SIGQUIT);

    exit(0);
  }
}

void fillArray () {
  for (int i = 0;  i < (sizeof(valor)/sizeof(valor[0])); i++) {
    valor[i] = -1;
  }
}
  //printf("Se esta ejecutando esta linea. Signal: %d\n", signal);
