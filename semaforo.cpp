#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <vector>
#include <stdlib.h>

using namespace std;

typedef struct
{
    vector<int> buffer;
    int maxProducts;
    int count;
}BUFFER;

BUFFER* products;
sem_t full, empty, mutex;
pthread_t* producers;
pthread_t* consumers;

int nProduced, nConsumed = 0;

void* produce(void* arg)
{
    BUFFER* auxBuffer = (BUFFER*) arg;
    while(1)
    {
        sem_wait(&empty);
        sem_wait(&mutex);
        if(auxBuffer->count < auxBuffer->maxProducts)
        {
            int item = rand() % 100;
            auxBuffer->buffer.push_back(item);
            auxBuffer->count++;
        }
        sem_post(&mutex);
        sem_post(&full);

        if(auxBuffer->count == auxBuffer->maxProducts)  return NULL;
    }
    return NULL;
}

void* consume(void* arg)
{
    BUFFER* auxBuffer = (BUFFER*) arg;
    while(1)
    {
        sem_wait(&full);
        sem_wait(&mutex);
        if(auxBuffer->count > 0)
        {   
            auxBuffer->buffer.pop_back();
        }
        sem_post(&mutex);
        sem_post(&empty);

        if(auxBuffer->count == auxBuffer->maxProducts)  return NULL;
    }
    return NULL;
}


int main(int argc, char* argv[])
{   
    srand(time(NULL));
    int bufferSize = atoi(argv[1]);
    int nProducers = atoi(argv[2]);
    int nConsumers = atoi(argv[3]);

    producers = (pthread_t*) malloc(nProducers*sizeof(pthread_t));
    consumers = (pthread_t*) malloc(nConsumers*sizeof(pthread_t));
    products = (BUFFER*) malloc(sizeof(BUFFER));
    products->maxProducts = bufferSize;
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, 500);
    sem_init(&full, 0, 0);

    for(int i = 0; i < nProducers; i++) pthread_create(&producers[i], NULL, produce, (void *) products);
    for(int  i = 0; i < nConsumers; i++) pthread_create(&consumers[i], NULL, consume, (void *) products);
    for(int i = 0; i < nProducers; i++) pthread_join(producers[i], NULL);
    for(int i = 0; i < nConsumers; i++) pthread_join(consumers[i], NULL);

    free(producers);
    free(consumers);
    free(products);

    return 0;
}