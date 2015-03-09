/*
 ============================================================================
 Project     : CS 149 HW 3
 Authors     : Gabriel Orellana & Olin Wong
 Class	     : CS 149, Section 6
 Term	     : Spring 2015
 San Jose State University
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_RIDERS_N	5 // Number of riders
#define NUM_DRIVERS_M	1 // Number of drivers
#define MAIN_SLEEP_T	2 // Time the main method will run
#define L				3 // The size of the queue

int queueCount = 0;
int waitingQueue[L];
pthread_mutex_t lock;

void *rider_function(void *arg) // function for the riders
{
	pthread_mutex_lock(&lock);
	printf("Rider Created\n");
	if(queueCount < L)
	{
		waitingQueue[queueCount] = -1; // -1 as a sign that threre's a thread there
		queueCount++;
		printf("Rider added to queue\n");
		sleep(1);
	}
	else
	{
		while(queueCount >= L)
		{
			printf("waiting for space in queue\n");
			sleep(1);
		}
	}
	pthread_mutex_unlock(&lock);
	return NULL;
}
void *driver_function(void *arg) // function for the drivers
{
	pthread_mutex_lock(&lock);
	printf("Driver arrives\n");
	if(queueCount > 0)
	{
		int x;
		printf("takes rider\n");
		sleep(1);
		for(x = 0; x <= L; x++)
		{
			if(waitingQueue[x] != -1)
			{
				waitingQueue[x] = NULL;
				queueCount--;
				break;
			}
		}
	}
	else
	{
		while(queueCount == 0)
		{
			printf("waiting for riders\n");
			sleep(1);
		}
	}
	pthread_mutex_unlock(&lock);
	return NULL;
}

int main(void)
{
	pthread_t tidRiders[NUM_RIDERS_N]; // tid for riders
	pthread_t tidDrivers[NUM_DRIVERS_M]; // tid for drivers

	int i; // counter for riders
	int j; // counter for drivers

	if (pthread_mutex_init(&lock, NULL) != 0) // creating the mutex
	    {
	        printf("\n mutex init failed\n");
	        return 1;
	    }
	if(NUM_RIDERS_N >= NUM_DRIVERS_M)
	{

		for(i = 0; i < NUM_RIDERS_N; i++) // used to create the rider threads
		{
			if ( pthread_create( &tidRiders[i], NULL, rider_function, NULL) ) {
				printf("error creating thread.");
				abort();
				}

			if ( pthread_join ( tidRiders[i], NULL ) ) {
				printf("error joining thread.");
				abort();
	  			}
			if(i < NUM_DRIVERS_M)
			{
				if ( pthread_create( &tidDrivers[i], NULL, driver_function, NULL) ) {
						printf("error creating thread.");
						abort();
						}
				if ( pthread_join ( tidDrivers[i], NULL ) ) {
						printf("error joining thread.");
						abort();
						}
			}
		}
	}
	else
	{
		for(j = 0; j < NUM_DRIVERS_M; j++) // used to create the driver threads
		{
			if ( pthread_create( &tidDrivers[j], NULL, driver_function, NULL) ) {
				printf("error creating thread.");
				abort();
				}
			if ( pthread_join ( tidDrivers[j], NULL ) ) {
				printf("error joining thread.");
				abort();
				}
			if(j < NUM_RIDERS_N)
			{
				if ( pthread_create( &tidRiders[i], NULL, rider_function, NULL) ) {
					printf("error creating thread.");
					abort();
					}

				if ( pthread_join ( tidRiders[i], NULL ) ) {
					printf("error joining thread.");
					abort();
					}
			}
		}
	}
	pthread_mutex_destroy(&lock); // destroying the mutex
	  return 0;
}
