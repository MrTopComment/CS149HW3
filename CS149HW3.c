/*
 ============================================================================
 Project     : CS 149 HW 3
 Authors     : Gabriel Orellana & Olin Wong
 Class		 : CS 149, Section 6
 Term		 : Spring 2015
 San Jose State University
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define NUM_RIDERS_N	5 // Number of riders
#define NUM_DRIVERS_M	5 // Number of drivers
#define L				5 // The size of the queue
#define T				20 // The time the main program runs

int ridersServed = 0;
int queueCount = 0;
int IDRider[NUM_RIDERS_N+1000];
int IDDriver[NUM_DRIVERS_M+1000];
int waitingQueue[L];
int threadCountRiders = 0;
int threadCountDrivers = NUM_RIDERS_N;
pthread_mutex_t lock;

void exitfunc(int sig)
{
	printf("Number of Riders Served: %i\n",ridersServed);
	printf("Riders remaining in the queue: %i\n",queueCount);
	_exit(1);
}

void *rider_function(void *arg) // function for the riders
{
	pthread_mutex_lock(&lock);
	IDRider[threadCountRiders] = threadCountRiders  + 1;
	printf("Rider %i Created\n",IDRider[threadCountRiders]);
	printf("Lenth of the queue: %i\n",queueCount);
	threadCountRiders++;
	if(queueCount < L)
	{
		waitingQueue[queueCount] = -1; // -1 as a sign that threre's a thread there
		queueCount++;
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
	sleep(rand()%5);
	pthread_mutex_unlock(&lock);
	return NULL;
}
void *driver_function(void *arg) // function for the drivers
{
	pthread_t tid;
	pthread_mutex_lock(&lock);
	IDDriver[threadCountDrivers] = threadCountDrivers + 1;
	printf("Driver %i arrives\n", IDDriver[threadCountDrivers]);
	printf("Lenth of the queue: %i\n",queueCount);
	if(queueCount > 0)
	{
		int x; // counter for the for loop
		printf("Driver %i riding\n", IDDriver[threadCountDrivers]);
		sleep(rand()%5);
		for(x = 0; x <= L; x++)
		{
			if(waitingQueue[x] != -1)
			{
				waitingQueue[x] = NULL;
				queueCount--;
				ridersServed++;
				threadCountRiders--;
				printf("Rider %i completed riding\n",IDRider[threadCountRiders]);
				threadCountRiders++;
				printf("Lenth of the queue: %i\n",queueCount);
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
	threadCountDrivers++;
	sleep(rand()%5);
	pthread_mutex_unlock(&lock);
	return NULL;
}

int main(int argc, char **argv)
{
	time_t startTime = time(NULL);
	printf("Welcome to the premier taxi service company ORANGE-SAUCE. \n");
	signal(SIGALRM, exitfunc);
	alarm(T);

	pthread_t tidRiders[NUM_RIDERS_N]; // tid for riders
	pthread_t tidDrivers[NUM_DRIVERS_M]; // tid for drivers

	int i; // counter for riders
	int j; // counter for drivers
    do {
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
	threadCountRiders = 0;
	threadCountDrivers = NUM_RIDERS_N;
	pthread_mutex_destroy(&lock); // destroying the mutex
    } while (time(NULL) < startTime + T);
	  return 0;
}
