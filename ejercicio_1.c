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
  char numero[20];
  char *p;


  printf("Ingrese el nivel del arbol de procesos: ");

  fgets(numero, 20, stdin);

  long int valor = strtol(numero, &p, 10);

  printf("%s\n", p);

  if (valor == 0) {
    printf("Ingrese un valor valido\n");
    exit(0);
  }

  imprimirNivel(valor);
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
