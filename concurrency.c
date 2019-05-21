#include <stdio.h>		// Standard Input-Output
#include <stdlib.h>		// Standard Functoins
#include <pthread.h>	// Threading Library
#include <string.h>
#define BUFFER 2		// Max BufferSize 

int buffer = 0;
int choice[2];
//int quantity[2];

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;	// mutex lock to avoid race conditions

void *producer(void *vargp){						// Producer Thread Function
	int my = (int)vargp;
	while(1){
		pthread_mutex_lock(&mutex);		// Check if resource is free.
		if(buffer < BUFFER){			// Check if the buffer is not full.
			buffer++;
			char* food;
			for(int i = 0; i < 2; i++) {
				do {
					choice[i] = (rand() % 3);
					
					if(choice[i] == 0)
						food = "hamburger";
					else if(choice[i] == 1)
						food = "fries";
					else
						food = "soda";
					
					///quantity[i] = (rand() % 10);
				} while(i > 0 && (choice[i] == choice[i-1]/* || quantity[i] == 0*/));
				printf("The chef produced %s.\n", food);
			}
			printf("Buffer Incremented^ \tby Cook: %d \tCurrent Size: %d\n",(my+1),buffer);
			//sleep(1);
		}
		else{
			// printf("Buffer OVERFLOW \t\t\tCurrent Size: %d\n",buffer );
		}
		pthread_mutex_unlock(&mutex);	// free the resource.
	}
	return NULL;
}

void *consumer(void *vargp){						// Consumer Function
	int my = (int)vargp;
    int count1, count2, count3 = 0;
	while(1){
		pthread_mutex_lock(&mutex);		// Check if resource is free.
		if(buffer > 0){					// Check if the buffer is not full.
			buffer--;
            if (my == 0)
		{
			count1++;
		}
		else if (my == 1)
		{
			count2++;
		}
		else if (my == 2)
		{
			count3++;
		}
			printf("Buffer Decremented \tby Customer: %d \tCurrent Size: %d\n",(my+1),buffer );
			// sleep(1);
		}
		else{
			// printf("Buffer UNDERFLOW \t\t\tCurrent Size: %d\n",buffer );
		}
		pthread_mutex_unlock(&mutex);	// free the resource.
    printf("Customer 1 ate: %d\n\n", count1);
	printf("Customer 2 ate: %d\n\n", count2);
	printf("Customer 3 ate: %d\n\n", count3);
	}
	return NULL;
}

int main(int argc, char const *argv[]){
	// command line input.
	const int cook = 1;//atoi(argv[1]);
	const int customers = 3;//atoi(argv[2]);
	pthread_t ptid[cook], ctid[customers];

	int i ;
	for (i = 0; i < cook; ++i){
		pthread_create(&ptid[i], NULL, producer, (void *)i);		// initialise all the threads.
	}

	for (i = 0; i < customers; ++i){
		pthread_create(&ctid[i], NULL, consumer, (void *)i);		// initialise all the threads.
	}

	for (i = 0; i < cook; ++i){
		pthread_join(ptid[i], NULL);
	}
	
	for (i = 0; i < customers; ++i){
		pthread_join(ctid[i], NULL);
	}
	return 0;
}