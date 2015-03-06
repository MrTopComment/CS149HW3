/*
 ============================================================================
 Project     : CS 149 HW 2
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
#define NUM_DRIVERS_M	4 // Number of drivers
#define MAIN_SLEEP_T	2 // Time the main method will run

void *rider_function(void *arg) // function for the riders
{
	printf("Olin I want Lavics\n");
	sleep(1);
	return NULL;
}
void *driver_function(void *arg) // function for the drivers
{
	printf("Yeah me too\n");
	sleep(1);
	return NULL;
}

int main(void)
{
	pthread_t tidRiders[NUM_RIDERS_N]; // tid for riders
	pthread_t tidDrivers[NUM_DRIVERS_M]; // tid for drivers

	int i; // counter for riders
	int j; // counter for drivers

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
	}

	for(j = 0; j < NUM_RIDERS_N - 1; j++) // used to create the driver threads
	{
		if ( pthread_create( &tidDrivers[j], NULL, driver_function, NULL) ) {
		  printf("error creating thread.");
		  abort();
		}

		if ( pthread_join ( tidDrivers[j], NULL ) ) {
		  printf("error joining thread.");
		  abort();
		}
	}
	  exit(0);
}
