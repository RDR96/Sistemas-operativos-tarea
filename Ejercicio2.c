#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "errno.h"
#include "sys/wait.h"
#include "string.h"

int factorial(int number, int cmp);

int main(int argc, char *argv[]){
  int number;
  printf("Favor de ingresar numero: ");
  scanf("%d", &number);
  printf("%d\n", factorial(number, number));

  return 0;
}

int factorial(int number, int cmp){
    int status;
    //crea procesos
    pid_t pid = fork();
    //Condicion de paro
    if((number==1) && pid==0)
      exit(1);
    //Proceso Padre espera hijos
    waitpid(-1, &status, 0);
    //Verifica si el hijo termino normalmente
    if(WIFEXITED(status)){
      //Devuelve el valor de salida del hijo
      int value = WEXITSTATUS(status);
      if(number==cmp)
        return number*value;
      else
        exit(number*value);
    }
    //Aplicando Recursion
    factorial(number-1, cmp);
}
