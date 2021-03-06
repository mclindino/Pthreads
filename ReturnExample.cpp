#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

void *function_thread(void *vector);

int aux_div;
int start = 0;

int main()
{
	int *array, *sum;
	int t,m,div;
	int result = 0;
	pthread_t *id;

	cout << "Configurar parametro t: ";
	cin >> t;
	
	cout << "Configurar parametro m: ";
	cin >> m;

	array = (int *) malloc(t * sizeof(int));
	id = (pthread_t *) malloc(m * sizeof(pthread_t));
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

	for(int i = 0; i < m; i++)
	{
		pthread_create(&id[i], NULL, function_thread, (void*)array);
	}

 	for(int i = 0; i < m; i++)
 	{
 		pthread_join(id[i], (void**)&sum);
 		result = result + *sum;
 	}

 	cout << "Result: " << buff << endl;
 	free(array);
 	free(id);
 	return 0;
}

void *function_thread(void *vector)
{
	int *sum;
	int *array;
	int i;
	array = (int*)vector;
	sum = malloc(1 * sizeof(int));
	*sum = 0;

	for(i = start; i < (start + aux_div); i++)
	{
		*sum = *sum + array[i];
	}
	start = i;
	return sum;
}