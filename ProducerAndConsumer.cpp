#include <iostream>
#include <list>
#include <pthread.h>

using namespace std;

//CRIA STRUCT REFERENTE AO BUFFER
typedef struct
{
	list<int> *buffer;
	int v, t; 
}BUFFER, *thread_BUFFER;

//CRIA AS FUNCOES PARA PRODUZIR E CONSUMIR
void *produce(void * arg);
void *consume(void * arg);

//INSTANCIA VARIAVEIS NECESSARIAS PARA AS THREADS
BUFFER *buff;
pthread_t *producers;
pthread_t *consumers;
pthread_mutex_t lock;
pthread_cond_t full_buffer;
pthread_cond_t empty_buffer;

int main(int argc, char *argv[])
{	
	int v, p, c, t;
	list<int> array;

	srand(time(NULL));

	//RETIRA OS VALORES DESEJADOS
	v = atoi(argv[1]);
	p = atoi(argv[2]);
	c = atoi(argv[3]);
	t = atoi(argv[4]);

	//ALOCA A QUANTIDADE DE THREADS E TAMANHO DO BUFFER
	producers = (pthread_t *) malloc(p * sizeof(pthread_t));
	consumers = (pthread_t *) malloc(c * sizeof(pthread_t));
	buff = (thread_BUFFER) malloc(sizeof(BUFFER));
	buff[0].buffer = &array;
	buff[0].v = v;
	buff[0].t = t;

	//INICIALIZA MUTEX E VARIAVEIS DE CONDICOES
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&full_buffer, NULL);
	pthread_cond_init(&empty_buffer, NULL);

	//DISPARA THREADS PRODUTORES
	for(int i = 0; i < p; i++)
	{
		pthread_create(&producers[i], NULL, produce, &buff[0]);			
	}

	//DISPARA THREADS CONSUMIDORES
	for(int i = 0; i < c; i++)
	{
		pthread_create(&consumers[i], NULL, consume, &buff[0]);			
	}

	//RETORNA AS THREADS PRODUTORES PARA A MAIN
	for(int i = 0; i < p; i++)
	{
		pthread_join(producers[i], NULL);
	}

	//COLOCA -1 NO BUFFER
	for(int i = 0; i < c ; i++){
        buff[0].buffer->push_back(-1);
        pthread_cond_signal(&full_buffer);
    }

    //RETORNA AS THREADS PRODUTORES PARA A MAIN
	for(int i = 0; i < c; i++)
	{
		pthread_join(consumers[i], NULL);
	}

	return 0;
}

void *produce(void * arg)
{
	thread_BUFFER list = (thread_BUFFER) arg;
	
	for(int i = 0; i < list->v; i++)
	{
		//PRENDE O MUTEX PARA MANIPULAR O BUFFER
		pthread_mutex_lock(&lock);
		
		//CASO O BUFFER ESTIVER CHEIO, ESPERA O CONSUMIDOR DISPARAR UM SINAL PARA CONTINUAR A PRODUZIR
		while(list->buffer->size() == list->t)
		{
			pthread_cond_wait(&empty_buffer, &lock);
		}

		//COLOCA O ITEM PRODUZIDO ALEATORIAMENTE NO BUFFER
		list->buffer->push_back(rand());

		//EMITE UM SINAL DIZENDO QUE O BUFFER NAO ESTA VAZIO, OU SEJA, TEM UM PRODUTO NELE
		pthread_cond_signal(&full_buffer);

		//LIBERA O MUTEX
		pthread_mutex_unlock(&lock);
	}

	return NULL;
}

void *consume(void * arg)
{
	thread_BUFFER list = (thread_BUFFER) arg;
	int value;
	bool prime;

	while(1)
	{

		//PRENDE O MUTEX PARA MANIPULAR O BUFFER
		pthread_mutex_lock(&lock);
		
		//CASO O BUFFER ESTEJA VAZIO, ESPERA O PRODUTOR DISPARAR UM SINAL PARA CONTINUAR A CONSUMIR
		while(list->buffer->size() == 0)
		{
			pthread_cond_wait(&full_buffer, &lock);
		}

		//RETIRA O PRIMEIRO ITEM DO BUFFER
		value = list->buffer->front();
		list->buffer->pop_front();

		//CASO ESSE ITEM FOR -1, ACABA O LACO INFINITO
		if(value == -1)
		{
			pthread_mutex_unlock(&lock);
			break;
		} else
		{
			prime = true;

			//CASO O VALOR NO BUFFER NAO SEJA -1, VERIFICA SE ESSE NUMERO E PRIMO
		  	for(int i = 2; i <= (int)(value/2); i++)
		  	{
        	   	if(value % i == 0)
        	   	{
        	   		prime = false;
        	   		break;
        	   	}

            }

            //SE O NUMERO CONSUMIDO FOR PRIMO, IMPRIME NA TELA O ID DA THREAD E O NUMERO
            if(prime)
            {
            	cout << "[" << pthread_self() << ":" << value << "]" << endl;
            }
        }

        //EMITE UM SINALDIZENDO QUE O BUFFER NAO ESTA CHEIO, OU SEJA, PODE-SE PRODUZIR MAIS ITENS
		pthread_cond_signal(&empty_buffer);
		
		//LIBERA O MUTEX
		pthread_mutex_unlock(&lock);
	}

	return NULL;
}