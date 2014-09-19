/*
Arukgoda J.S.
100040X

CS 4532
Lab 1
Parallel Program with one mutex
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"
#include <pthread.h>

// Constants
const int MAX = 65536;

// Global Variables
pthread_mutex_t mutex;

struct list_node_s** head; 
	
/* 	n 	- number of random numbers
	m 	- number of operations
	memb	- member operation fraction 
	ins	- insert operation fraction
	del	- delete operation fracrion
	tc	- thread count
	n_obs	- number of observations
*/
int n;
int m;
float memb;
float ins;
float del;
int n_obs;
int tc;

int *data;

// Link list node
struct list_node_s {
	int data;
	struct list_node_s* next;
};

// Method declaration
int Member (int value, struct list_node_s* head_p);
int Insert (int value, struct list_node_s** head_pp);
int Delete (int value, struct list_node_s** head_pp);
void Random (int *arr, int count);
void Populate (int *data);
void *Execute (void *rank);
void PrintStat (double *stat, int n_obs);

//----------------------------------------------------------------------------------------------------------------------------------------//
// Main method

int main(int argc, char* argv[]) {
	long thread;
	pthread_t* thread_handles;

	n 	= strtol (argv[1], NULL, 10);
	m 	= strtol (argv[2], NULL, 10);
	memb 	= strtof (argv[3], NULL);
	ins 	= strtof (argv[4], NULL);
	del	= strtof (argv[5], NULL);
	n_obs	= strtol (argv[6], NULL, 10);
	tc	= strtol (argv[7], NULL, 10);

	data = malloc((n+m)*sizeof(int));
 	printf ("Input Arguments n = %d m = %d memb = %f ins = %f del = %f n_obs = %d tc = %d \n", n, m, memb, ins, del, n_obs, tc);

	int count;
	
	double stat[n_obs];
	double start, end;

	for (count = 0; count < n_obs; count++) {
		head = malloc(sizeof(struct list_node_s));
		
		Random (data, n + m);
		
		Populate(data);
		
		thread_handles = malloc (tc * sizeof(pthread_t));
		pthread_mutex_init(&mutex, NULL);
		
		GET_TIME(start);
		for (thread = 0; thread < tc; thread++) {
			pthread_create(&thread_handles[thread], NULL, Execute, (void*) thread);
		}

		for(thread = 0; thread < tc; thread++) {
			pthread_join(thread_handles[thread], NULL);
		}
		GET_TIME(end);
		
		pthread_mutex_destroy(&mutex);
		free(thread_handles);

		stat[count] = end - start;		
	}

	PrintStat (stat, n_obs);

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------//
// Member

int Member (int value, struct list_node_s* head_p) {

	struct list_node_s* curr_p = head_p;

	while (curr_p != NULL && curr_p -> data < value ) {
		curr_p = curr_p -> next;
	}

	if (curr_p == NULL || curr_p -> data > value) {
		return 0;
	} else {
		return 1;
	}		
}

//----------------------------------------------------------------------------------------------------------------------------------------//
//Insert

int Insert (int value, struct list_node_s** head_pp) {

	struct list_node_s* curr_p = *head_pp;
	struct list_node_s* pred_p = NULL;
	struct list_node_s* temp_p;

	while (curr_p != NULL && curr_p -> data < value) {
		pred_p = curr_p;
		curr_p = curr_p -> next;
	}

	if (curr_p == NULL || curr_p -> data > value) {
		temp_p = malloc(sizeof(struct list_node_s));
		temp_p -> data = value;
		temp_p -> next = curr_p;
		
		if (pred_p == NULL)
			*head_pp = temp_p;
		else 
			pred_p -> next = temp_p;
		
		return 1;	
	} else {
		return 0;
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------//
//Delete

int Delete (int value, struct list_node_s** head_pp) {

	struct list_node_s* curr_p = *head_pp;
	struct list_node_s* pred_p = NULL;

	while (curr_p != NULL && curr_p -> data <value) {
		pred_p = curr_p;
		curr_p = curr_p -> next;
	}

	if (curr_p != NULL && curr_p -> data == value) {
		if (pred_p == NULL) {
			*head_pp = curr_p -> next;
			free(curr_p);
		} else {
			pred_p -> next = curr_p -> next;
			free(curr_p);
		}

		return 1;
	} else {
		return 0;
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------//
// Random

void Random (int *arr, int count) {

	int i;
 	int random [MAX];

	for (i = 0; i < MAX; i++) {
		random[i] = i;
	}

	for (i = 0; i < MAX; i++) {
		int swap = rand() % MAX;

		int temp = random[swap];

		random[swap] = random[i];
		random[i] = temp;
	}

	for (i = 0; i < count; i++) {
		arr[i] = random[i];
	}	
}

//----------------------------------------------------------------------------------------------------------------------------------------//
// Populate
void Populate (int *data) {
	
	int i;

	for (i = 0; i < n; i++) {
		int p = Insert (data[i], head);
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------//
// Execute

void *Execute (void *rank) {

	// Randomly execute the operations
	int member_ops = m * memb / tc;
	int insert_ops = m * ins / tc;
	int delete_ops = m * del / tc;

	int i = n;

	while (member_ops > 0 || insert_ops > 0 || delete_ops > 0) {
		int temp = rand() % 3;
		
		if (temp == 0 && member_ops > 0) { // Do member operation
			pthread_mutex_lock(&mutex);
			Member (data[i], *head);
			pthread_mutex_unlock(&mutex);
			member_ops--;
			i++;
		} else if (temp == 1 && insert_ops > 0) { // Do insert operation
			pthread_mutex_lock(&mutex);
			Insert (data[i], head);
			pthread_mutex_unlock(&mutex);
			insert_ops--;
			i++;
		} else if (temp == 2 && delete_ops > 0) { // Do delete operaion
			pthread_mutex_lock(&mutex);
			Delete (data[i], head);
			pthread_mutex_unlock(&mutex);
			delete_ops--;
			i++;
		} else {
			continue;
		}
	}
	
}

//----------------------------------------------------------------------------------------------------------------------------------------//
// PrintStat

void PrintStat (double *stat, int n_obs) {

	double mean, sd;
	int i; 
	double count = 0;
	
	for (i = 0; i < n_obs; i++) {
		count +=stat[i];	
	}	

	mean = count / n_obs;
	
	count = 0;

	for (i = 0; i < n_obs; i++) {
		count += (stat[i]-mean)*(stat[i]-mean);
	} 

	sd = sqrt(count/n_obs);

	printf("Average	: %f\n", mean);
	printf("SD	: %f\n",sd);
}





