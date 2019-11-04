#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdbool.h>

sigset_t mask, pending;



void signal_handler (int sig) {


    if (sig == SIGINT) {
      printf("\nSeñales bloqueadas:\n");
      sigpending(&pending);
      for (int i = 1; i <= 20; i++) {
        if(sigismember(&pending, i))
            fprintf(stderr, "INT: %d, %s\n", i, strsignal(i));
      }
    }

}

int main() {
    int valor;
    bool corriendo = true;

    sigemptyset(&mask);

    for (int i = 1; i <= 60; i++) {
      if (i != SIGINT)
        sigaddset(&mask, i);
    }

    //sigaddset(&mask, SIGTERM);
    //sigaddset(&mask, SIGQUIT);
    //sigaddset(&mask, SIGTSTP);
    //sigaddset(&mask, SIGSEGV);
    sigprocmask(SIG_BLOCK, &mask, 0);

    signal(SIGINT, signal_handler);

    char numeros[100];
    char* ptr;
    printf("Ingrese 2 para ver seniales bloqueadas: \n");
    while (corriendo) {
      fprintf(stderr, "Ingrese el numero de la senial a bloquear: ");

      fgets(numeros, 100, stdin);

      long int valorN = strtol(numeros, &ptr, 10);

      if (valorN == 0) {
        printf("Ingrese un numero de 1 a 60\n");
        exit(0);
      }
      kill(getpid(), valorN);
    }

    return 0;
}
