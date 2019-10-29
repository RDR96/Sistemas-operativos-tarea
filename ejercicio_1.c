#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>


void imprimirNivel(int numProc);
void imprimirTabulacion(int nivelTabulacion);

int main() {
  int numberOfProcccess;

  printf("Ingrese el nivel del arbol de procesos: ");
  scanf("%d", &numberOfProcccess);

  imprimirNivel(numberOfProcccess);
  wait(NULL);

  return 0;
}

void imprimirNivel(int numProc) {
  printf("Nivel: %d, %d\n", 0, getpid());

  for (int i = 0; i < numProc; i++) {
    fork();

    imprimirTabulacion(i + 1);

    printf("Nivel: %d, %d\n", i + 1, getpid());
    //usleep(9000); // Argumento en microsegundos.
    sleep(1); // Argumento en segundos.
  }
}

void imprimirTabulacion(int nivelTabulacion) {
  for (size_t i = 0; i < nivelTabulacion; i++) {
    printf("\t");
  }
}
