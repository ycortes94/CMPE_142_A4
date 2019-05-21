#include <stdio.h>		// Standard Input-Output
#include <stdlib.h>		// Standard Functoins
#include <pthread.h>	// Threading Library
#include <string.h>
#define BUFFER 2		// Max BufferSize 

int buffer = 0;
int choice[2];
int quantity[2];
char* food;

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;	// mutex lock to avoid race conditions

void *producer(void *vargp){						// Producer Thread Function
	int my = (int)vargp;
	while(1){
		pthread_mutex_lock(&mutex);		// Check if resource is free.
		for(int i = 0; i < 2; i++) {
			do {
				choice[i] = (rand() % 3);
				
				if(choice[i] == 0)
					food = "hamburger(s)";
				else if(choice[i] == 1)
					food = "fries";
				else
					food = "soda";
				
				quantity[i] = (rand() % 10);
			} while(i > 0 && (choice[i] == choice[i-1] || quantity[i] == 0));
			printf("The chef produced %d %s.\n", quantity[i], food);
		}
		pthread_mutex_unlock(&mutex);	// free the resource.
	}
	return NULL;
}

void *consumer(void *vargp){						// Consumer Function
	int my = (int)vargp;
    int count1_h, count1_f, count1_s, count2_h, count2_f, count2_s, count3_h, count3_f, count3_s = 0;

	printf("Choice %d %d.\n", choice[0], quantity[0]);
	printf("Choice %d %d.\n\n", choice[1], quantity[1]);
	while(1) {
		pthread_mutex_lock(&mutex);		// Check if resource is free.
		if(quantity[0] > 0 && quantity[1] > 0) {
			if(choice[0] == 0 || choice[1] == 0) { // hamburger
				if(choice[0] == 0)
					quantity[0]--;
				else
					quantity[1]--;
				if(my == 1)
					count1_h++;				
				else if(my == 2)
					count2_h++;
			} else if(choice[0] == 1 || choice[1] == 1) { // fries
				if(choice[0] == 1)
					quantity[0]--;
				else
					quantity[1]--;
				if(my == 0)
					count1_f++;				
				else if(my == 2)
					count2_f++;
			} else { // soda
				if(choice[0] == 2)
					quantity[0]--;
				else
					quantity[1]--;
				if(my == 0)
					count1_s++;				
				else if(my == 1)
					count2_s++;
			}
		}
		pthread_mutex_unlock(&mutex);	// free the resource.
		printf("\n\nCustomer 1 ate: %d hamburgers, %d fries, and %d soda.\n", count1_h, count1_f, count1_s);
		printf("Customer 2 ate: %d hamburgers, %d fries, and %d soda.\n", count2_h, count2_f, count2_s);
		printf("Customer 3 ate: %d hamburgers, %d fries, and %d soda.\n", count3_h, count3_f, count3_s);
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