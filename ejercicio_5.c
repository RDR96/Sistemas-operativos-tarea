#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>

typedef struct {
  long int parentId;
  int number;
  sigset_t *sig;
}nodo;


void* doSomething(void *args) {
  sigset_t sig;
  int number;
  nodo *prueba = (nodo*)args;

   int s, sigVal;

   while (1) {
       s = sigwait(prueba->sig, &sigVal);

       if (sigVal == SIGUSR1){
         printf("id Hijo #%d: %lu\n", prueba->number, pthread_self());
         sleep(1);
         pthread_kill(prueba->parentId, SIGUSR1);
       }
       if (sigVal == SIGTERM) {
         printf("sALIO:  id Hijo #%d: %lu\n", prueba->number, pthread_self());
         pthread_kill(prueba->parentId, SIGTERM);
         pthread_exit(NULL);
       }
   }

}

int order = 0;
int valorCondicion = 1;

void changeDirection (int signal) {
  order = !order;
}
void quitT() {
  valorCondicion = 0;
}

int parentId;

int main () {

  sigset_t sig;
  int number;
  int numeroHilos;
  int s;
  int s1, sigVal1;
  int counter = 0;

  signal(SIGTSTP, changeDirection);
  signal(SIGINT, quitT);


  fprintf(stderr, "Cuantos hilos desea crear?");
  scanf("%d", &numeroHilos);


  pthread_t arrayOfThreads[numeroHilos];
  sigemptyset(&sig);
  sigaddset(&sig, SIGUSR1);
  sigaddset(&sig, SIGTERM);
  s = pthread_sigmask(SIG_BLOCK, &sig, NULL);
  parentId = pthread_self();

  for (int i = 0; i < numeroHilos; i++) {
    nodo *argss = (nodo *)malloc(sizeof(nodo));
    argss->parentId = pthread_self();
    argss->number = i;
    argss->sig = &sig;
    pthread_create(&arrayOfThreads[i], NULL, doSomething, argss);

  }


  while (valorCondicion){
    if (counter ==  numeroHilos)
      counter = 0;

    if (counter < 0)
      counter = numeroHilos - 1;

      pthread_kill(arrayOfThreads[counter], SIGUSR1);
      sleep(1);
      s1 = sigwait(&sig, &sigVal1);

    if (order == 0 )
      counter++;
    else
      counter--;
  }

  for(int i = 0; i < numeroHilos; i++) {
    pthread_kill(arrayOfThreads[i], SIGTERM);
    s1 = sigwait(&sig, &sigVal1);
  }


  return 0;
}
