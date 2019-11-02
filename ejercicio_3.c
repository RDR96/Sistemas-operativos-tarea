#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX 256

int main (int argc, char *argv[], char *env[]) {
  //char cadena[50];
  //fprintf(stderr, "¿Que programa desea ejecutar?");
  //scanf("%s", &cadena[0]);
  //printf("%s\n", cadena);
  while (1) {
    int proceso = vfork();
    int contador = 0;

    if (proceso == 0) {
        for (int k = 0; env[k]; k++) {
          contador++;
        }

        char varAux[MAX];
        char* cadena[20];
        char* token;
        char* texto;
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

    }
    wait(NULL);
  }





  return 0;
}
