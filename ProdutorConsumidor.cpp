#include <iostream>
#include <list>
#include <pthread.h>

using namespace std;

void *produce(void * arg);
void *consume(void * arg);

int t, v;
pthread_t *producers;
pthread_t *consumers;
pthread_mutex_t lock;
pthread_cond_t full_buffer;
pthread_cond_t empty_buffer;

int main()
{	
	int p, c;
	list<int> *buffer;
	list<int>::iterator item;

	srand(time(NULL));
	
	cout << "Interactions: ";
	cin >> v;

	cout << "Producers: ";
	cin >> p;

	cout << "Consumers: ";
	cin >> c;

	cout << "Buffer size: ";
	cin >> t;

	producers = (pthread_t *) malloc(p * sizeof(pthread_t));
	consumers = (pthread_t *) malloc(c * sizeof(pthread_t));

	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&full_buffer, NULL);
	pthread_cond_init(&empty_buffer, NULL);

	//DISPARA THREADS PRODUTORES
	for(int i = 0; i < p; i++)
	{
		pthread_create(&producers[i], NULL, produce, (void*) buffer);			
	}

	//DISPARA THREADS CONSUMIDORES
	for(int i = 0; i < c; i++)
	{
		pthread_create(&consumers[i], NULL, consume, (void*) buffer);			
	}


	for(int i = 0; i < p; i++)
	{
		pthread_join(producers[i], NULL);
	}

	for(int i = 0; i < c ; i++){
        buffer->push_back(-1);
        pthread_cond_signal(&full_buffer);
    }

	for(int i = 0; i < c; i++)
	{
		pthread_join(consumers[i], NULL);
	}

	return 0;
}

void *produce(void * arg)
{
	list<int> *buffer = (list<int> *) arg;
	
	for(int i = 0; i < v; i++)
	{
		pthread_mutex_lock(&lock);
		
		while(buffer->size() == t)
		{
			pthread_cond_wait(&empty_buffer, &lock);
		}

		buffer->push_back(rand());

		pthread_cond_signal(&full_buffer);

		pthread_mutex_unlock(&lock);
	}

	return NULL;
}

void *consume(void * arg)
{
	list<int> *buffer = (list<int> *) arg;
	int value;

	while(1)
	{
		pthread_mutex_lock(&lock);
		
		while(buffer->size() == 0)
		{
			pthread_cond_wait(&full_buffer, &lock);
		}

		if(buffer->front() == -1)
		{
			buffer->pop_front();
			pthread_mutex_unlock(&lock);
			break;
		}

		pthread_cond_signal(&empty_buffer);
		
		pthread_mutex_unlock(&lock);
	}

	return NULL;
}