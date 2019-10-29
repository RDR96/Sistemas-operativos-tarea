#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdbool.h>

sigset_t mask, pending;

void showMenu();

void signal_handler (int sig) {
    
    printf("\n");
    showMenu();
    if (sig == SIGINT) {
      printf("\nSeñales bloqueadas:\n");

      sigpending(&pending);
      for (int i = 1; i <= 20; i++) {
        if(sigismember(&pending, i))
            printf("INT: %d, %s\n", i, strsignal(i));
      }
    }

}

int main() {
    int valor;
    bool corriendo = true;

    sigemptyset(&mask);

    sigaddset(&mask, SIGTERM);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGTSTP);
    sigaddset(&mask, SIGSEGV);
    sigprocmask(SIG_BLOCK, &mask, 0);

    signal(SIGINT, signal_handler);

    while (corriendo) {
        showMenu();
        scanf("%d", &valor);
        switch (valor) {
            case 1:
              kill(getpid(), SIGTERM);
              printf("\nParece que no es posible terminar el programa\n");
            break;

            case 2:
              kill(getpid(), SIGQUIT);
              printf("\nParece que no es posible terminar el programa\n");
            break;

            case 3:
              kill(getpid(), SIGTSTP);
              printf("\nParece que no es posible terminar el programa\n");
            break;

            case 4:
              kill(getpid(), SIGSEGV);
              printf("\nParece que no es posible terminar el programa\n");
            break;

        }
    }

    return 0;
}

void showMenu() {
  printf("\n1-Terminar el proceso: SIGTERM\n");
  printf("2-Salir del programa: SIGQUIT\n");
  printf("3-Parar el programa: SIGTSTP\n");
  printf("4-Violacion de segmento: SIGSEGV\n");
  printf("\nPuede realizar los siguientes comandos: \n");
  printf("Control Z: Detener el proceso\n");
  printf("Control \\: Salir del proceso\n");
  printf("Control C: Mostrar seniales bloqueadas\n");
}
