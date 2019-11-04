#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

int max = 1000;
int i, j, aux;
pthread_mutex_t llave, llave2;

typedef struct
{
	int n[1000];
	int cn;
} vector;

void *pedir(void *args)
{
	pthread_mutex_lock(&llave);
	vector *vect=(vector *)args;
	printf("Dimension del Vector: \n");
	int valor;
	scanf("%d", &vect->cn);

	for (i = 0; i < vect->cn; i++)
	{
		printf("Ingrese el Elemento #%d: \n", i);
		scanf("%d", &vect->n[i]);
	}
	pthread_mutex_unlock(&llave);

}

void *ordenar(void *args)
{



	vector *vect=(vector *)args;

	for (i = 1; i < vect->cn; i++)
	{

		for (j = 0; j < vect->cn - i; j++)
		{
			pthread_mutex_lock(&llave);
			if (vect->n[j] > vect->n[j + 1])
			{
				aux = vect->n[j + 1];
				vect->n[j + 1] = vect->n[j];
				vect->n[j] = aux;
			}
			pthread_mutex_unlock(&llave);
		}

	}

	pthread_mutex_lock(&llave2);
	printf("\nNormal\n");
	for (i = 0; i < vect->cn; i++)
		printf("%d\n", vect->n[i]);
	pthread_mutex_unlock(&llave2);


}

void *ordenarInverso(void *args)
{

	vector *vect=(vector *)args;

	for (i = 0; i < vect->cn - 1; i++)
	{

		for (j = vect->cn - 1; j >= i + 1; j--)
		{
			pthread_mutex_lock(&llave);
			if ( vect->n[j - 1] > vect->n[j])
			{
				aux = vect->n[j];
				vect->n[j] = vect->n[j - 1];
				vect->n[j - 1] = aux;
			}
			pthread_mutex_unlock(&llave);
		}

	}

	pthread_mutex_lock(&llave2);
	printf("\nInverso\n");
	for (i = 0; i < vect->cn; i++)
		printf("%d\n", vect->n[i]);
	pthread_mutex_unlock(&llave2);


}

int main(int argc, char *argv[])
{
	pthread_t idHilo1, idHilo2, idHilo3;

	pthread_mutex_init(&llave, NULL);
	pthread_mutex_init(&llave2, NULL);

	vector *vect = (vector * )malloc(sizeof(vector));
	pthread_create(&idHilo1, NULL, pedir, (void *)vect);
	pthread_join(idHilo1, NULL);

	pthread_create(&idHilo2, NULL, ordenar, (void *)vect);
	pthread_create(&idHilo3, NULL, ordenarInverso, (void *)vect);

	pthread_join(idHilo2, NULL);
	pthread_join(idHilo3, NULL);

	free(vect);
	return 0;
}
