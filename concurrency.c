#include <stdio.h>		// Standard Input-Output
#include <stdlib.h>		// Standard Functoins
#include <pthread.h>	// Threading Library
#define BUFFER 3		// Max BufferSize 


int buffer = 0;
int count = 0;

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;	// mutex lock to avoid race conditions

void *producer(void *vargp){						// Producer Thread Function
	int cook = (int)vargp;
	cook = cook + 1;
	// if(buffer !=0)
	//  {
		while(count != 100 /*buffer != 0*/)
		{
		pthread_mutex_lock(&mutex);		// Check if resource is free.
		if(buffer < BUFFER){			// Check if the buffer is not full.
			buffer++;
			printf("Buffer Incremented^ \tby Cook: %d \tCurrent Size: %d\n",cook,buffer );
			//sleep(1);
		}
		else{
			// printf("Buffer OVERFLOW \t\t\tCurrent Size: %d\n",buffer );
		}
		count++;
		pthread_mutex_unlock(&mutex);	// free the resource.
		}
	// } else {
	// 	pthread_cond_wait(, &mutex);
	// }
	
	return NULL;
}

void *consumer(void *vargp){						// Consumer Function
	int my = (int)vargp;
	while(count != 100 /*buffer == 0*/){
		pthread_mutex_lock(&mutex);		// Check if resource is free.
		if(buffer > 0){					// Check if the buffer is not full.
			buffer--;
			printf("Buffer Decremented \tby Customer: %d \tCurrent Size: %d\n",my,buffer );
			// sleep(1);
		}
		else{
			// printf("Buffer UNDERFLOW \t\t\tCurrent Size: %d\n",buffer );
		}
		count++;
		pthread_mutex_unlock(&mutex);	// free the resource.
	}
	return NULL;
}

int main(int argc, char const *argv[]){
	//Command line input.
    const int cook = 1; //atoi(argv[1]);
    const int customer = 3; //atoi(argv[2]);

	pthread_t ptid[cook], ctid[customer];

	int i ;
	for (i = 0; i < cook; ++i){
		pthread_create(&ptid[i], NULL, producer, (void *)i);		// initialise all the threads.
	}

	for (i = 0; i < customer; ++i){
		pthread_create(&ctid[i], NULL, consumer, (void *)i);		// initialise all the threads.
	}

	for (i = 0; i < cook; ++i){
		pthread_join(ptid[i], NULL);
	}
	
	for (i = 0; i < customer; ++i){
		pthread_join(ctid[i], NULL);
	}

	return 0;
}