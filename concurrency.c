#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include "common.h"
#include "common_threads.h"
#include "zemaphore.h"
// branch created

int max;
int loops;
int *buffer;

int use = 0;
int fill = 0;

sem_t empty;
sem_t full;
sem_t mutex;

#define CMAX (10)
int consumers = 1;

void do_fill(int value)
{
	buffer[fill] = value;
	fill++;
	if (fill == max)
		fill = 0;
}

int do_get()
{
	int tmp = buffer[use];
	use++;
	if (use == max)
		use = 0;
	return tmp;
}

void *producer(void *arg)
{
	int i;
	for (i = 0; i < loops; i++)
	{
		Sem_wait(&empty);
		Sem_wait(&mutex);
		do_fill(i);
		Sem_post(&mutex);
		Sem_post(&full);
	}

	// end case
	for (i = 0; i < consumers; i++)
	{
		Sem_wait(&empty);
		Sem_wait(&mutex);
		do_fill(-1);
		Sem_post(&mutex);
		Sem_post(&full);
	}

	return NULL;
}

void *consumer(void *arg)
{
	int tmp = 0;
	// int customer = (long long int) arg +1;
	int count1, count2, count3 = 0;
	while (tmp != -1)
	{
		Sem_wait(&full);
		Sem_wait(&mutex);
		tmp = do_get();
		Sem_post(&mutex);
		Sem_post(&empty);
		if (arg == 0)
		{
			count1++;
		}
		else if (arg == 1)
		{
			count2++;
		}
		else if (arg == 2)
		{
			count3++;
		}
		printf("Customer: %lld %d\n", (long long int)arg, tmp);
	}
	printf("Customer 1 ate: %d\n\n", count1);
	printf("Customer 2 ate: %d\n\n", count2);
	printf("Customer 3 ate: %d\n\n", count3);

	return NULL;
}

int main(int argc, char *argv[])
{
	max = 1;	   //atoi(argv[1]); Number of Cooks
	loops = 100;   //atoi(argv[2]);
	consumers = 3; //atoi(argv[3]); Number of Customers
	assert(consumers <= CMAX);

	buffer = (int *)malloc(max * sizeof(int));
	assert(buffer != NULL);
	int i;
	for (i = 0; i < max; i++)
	{
		buffer[i] = 0;
	}

	Sem_init(&empty, max); // max are empty
	Sem_init(&full, 0);	// 0 are full
	Sem_init(&mutex, 1);   // mutex
	pthread_t pid, cid[CMAX];

	Pthread_create(&pid, NULL, producer, NULL);

	for (i = 0; i < consumers; i++)
	{
		Pthread_create(&cid[i], NULL, consumer, (void *)(long long int)i);
	}

	Pthread_join(pid, NULL);

	for (i = 0; i < consumers; i++)
	{
		Pthread_join(cid[i], NULL);
	}

	return 0;
}