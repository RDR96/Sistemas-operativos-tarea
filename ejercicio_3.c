#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX 256

int main (int argc, char *argv[], char *env[]) {

  while (1) {

    int proceso;
    if ((proceso = vfork()) == -1) {
        printf("%d", errno);
    }

    int contador = 0;

    if (proceso == 0) {
        for (int k = 0; env[k]; k++) {
          contador++;
        }

        char varAux[MAX];
        char* cadena[20];
        char* token;
        char delim[4] = " \n\t";
        int i = 0;

        printf("<%s>: ", getenv("HOME"));

        fgets(varAux, MAX, stdin);

        token = strtok(varAux, delim);

        while (token != NULL) {

          if (i == 20) break;

          cadena[i] = token;
          token = strtok(NULL, delim);
          i++;
        }

        if (token == NULL && i < 20) {
          cadena[i] = NULL;
        }

        env[contador + 1] = NULL;

        execve(cadena[0], cadena, env);
        perror("No se pudo ejecutar el programa ");
    }
    wait(NULL);
  }

  return 0;
}
