#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

void *function_thread(void *vector);

int aux_div;
int start = 0;
pthread_mutex_t lock;
int buff = 0;

int main()
{
	int *array;
	int t,m,div;
	pthread_t *id;

	printf("Configurar parametro t: ");
	scanf("%d", &t);
	
	printf("Configurar parametro m: ");
	scanf("%d", &m);

	array = malloc(t * sizeof(int));
	id = malloc(m * sizeof(pthread_t));
	srand(time(NULL));

	for(int i = 0; i < t; i++)
	{
		array[i] = rand();
	}

	div = abs(t / m);
	aux_div = div;
	if(div == 0)
	{
		div = 1;
	}

	pthread_mutex_init(&lock, NULL);

	for(int i = 0; i < m; i++)
	{
		pthread_create(&id[i], NULL, function_thread, (void*)array);
	}

 	for(int i = 0; i < m; i++)
 	{
 		pthread_join(id[i], NULL);
 	}

 	printf("Result: %d\n", buff);
 	free(array);
 	free(id);
 	pthread_mutex_destroy(&lock);
 	return 0;
}

void *function_thread(void *vector)
{
	pthread_mutex_lock(&lock);

	int *array;
	int i;
	array = (int*)vector;

	for(i = start; i < (start + aux_div); i++)
	{
		buff = buff + array[i];
	}
	start = i;	

	pthread_mutex_unlock(&lock);
	return NULL;
}