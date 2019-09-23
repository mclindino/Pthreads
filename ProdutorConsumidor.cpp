#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

using namespace std;

void *produtor_function(void * arg);
void *consumidor_function(void * arg);

int *buffer;
int v, t, pos = 0;
pthread_mutex_t lock;
pthread_cond_t condC, condP;

int main()
{
	int p, c;
	pthread_t *produtores, *consumidores;
	srand(time(NULL));
	
	cout << "Quantidade de produtores: ";
	cin >> p;

	cout << "Quantidade de consumidores: ";
	cin >> c;

	cout << "Quantidade de interações: ";
	cin >> v;

	cout << "Tamanho do buffer: ";
	cin >> t;

	buffer = (int *) malloc(t * sizeof(int));
	produtores = (pthread_t *) malloc(p * sizeof(pthread_t));
	consumidores = (pthread_t *) malloc(c * sizeof(pthread_t));

	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&condC, NULL);
	pthread_cond_init(&condP, NULL);

	//DISPARA THREADS PRODUTORES
	for(int i = 0; i < p; i++)
	{
		pthread_create(&produtores[i], NULL, produtor_function, (void*)buffer);			
	}

	//DISPARA THREADS CONSUMIDORES
	for(int i = 0; i < c; i++)
	{
		pthread_create(&consumidores[i], NULL, consumidor_function, (void*)buffer);			
	}


	for(int i = 0; i < p; i++)
	{
		pthread_join(produtores[i], NULL);
	}

	for(int i = 0; i < c; i++)
	{
		pthread_join(consumidores[i], NULL);
	}

	return 0;
}

void *produtor_function(void * arg)
{
	
	for(int i = 0; i < v; i++)
	{
		pthread_mutex_lock(&lock);
		
		buffer[pos] = rand();
		cout << "Produzido: " << buffer[pos] << endl;
		pos++;
		pthread_cond_signal(&condC);

		while(buffer[t - 1] != 0)
		{
			pthread_cond_wait(&condP, &lock);
		}

		pthread_mutex_unlock(&lock);
	}
}

void *consumidor_function(void * arg)
{
	while(buffer[0] != 0)
	{
		pthread_mutex_lock(&lock);
		
		while(pos == 0)
		{
			pthread_cond_wait(&condC, &lock);
		}

		pos--;
		cout << "Consumido: " << buffer[pos] << endl;
		buffer[pos] = 0;
		pthread_cond_signal(&condP);
		
		pthread_mutex_unlock(&lock);
	}
}