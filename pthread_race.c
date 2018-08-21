#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
/**
 * THESE DEFINE VALUES CANNOT BE CHANGED.
 * DOING SO WILL CAUSE POINTS TO BE DEDUCTED
 * FROM YOUR GRADE
 */
 /** BEGIN VALUES THAT CANNOT BE CHANGED */
#define MAX_THREADS 10
#define MAX_ITERATIONS 25
/** END VALUES THAT CANNOT BE CHANGED */


/**
 * use this struct as a parameter for the function
 * nanosleep. 
 * For exmaple : nanosleep(&ts, NULL);
 */
struct timespec ts = {0, 99999};

int g_variable = 0;
void* adder(void*);
void* subtractor(void*);
pthread_mutex_t lock; // mutex lock

int
main(int argc, char** argv) {
	pthread_t tids[MAX_THREADS];
	pthread_mutex_init(&lock, NULL);
	for (int i = 0; i < MAX_THREADS; ++i) {
		if (i % 2 == 0) {
			int tid = i;
			pthread_create(&tids[i], NULL, adder, &tid);
		} else {
			int tid = i;
			pthread_create(&tids[i], NULL, subtractor, &tid);
		}
		nanosleep(&ts, NULL);
	}

	for (int i = 0; i < MAX_THREADS; ++i) {
		pthread_join(tids[i], NULL);
	}
	printf("Final Value of Shared Variable:\t%d\n", g_variable);

	pthread_mutex_destroy(&lock);
	return 0;
}

// thread function for adder threads
void* adder(void* param) {
	int tid = *((int *) param);
	int tmp;
	for (int i = 0; i < MAX_ITERATIONS; ++i) {
		pthread_mutex_lock(&lock); // acquire mutex lock
		tmp = g_variable;
		tmp++;
		nanosleep(&ts, NULL);
		g_variable = tmp;
		pthread_mutex_unlock(&lock); // release mutex lock
		printf("Current Value written to Global Variable by thread:\t%d is\t%d\n", tid, tmp);
		nanosleep(&ts, NULL);
	}
	pthread_exit(0);
}

// thread function for subtractor threads
void* subtractor(void* param) {
	int tid = *((int *) param);
	int tmp;
	for (int i = 0; i < MAX_ITERATIONS; ++i) {
		nanosleep(&ts, NULL);
		pthread_mutex_lock(&lock); // acquire mutex lock
		tmp = g_variable;
		tmp--;
		g_variable = tmp;
		pthread_mutex_unlock(&lock); // release mutex lock
		nanosleep(&ts, NULL);
		printf("Current Value written to Global Variable by thread:\t%d is\t%d\n", tid, tmp);
	}
	pthread_exit(0);
}
