/*
Arukgoda J.S.
100040X

CS 4532
Lab 1
Serial Program 
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"

// Constants
const int MAX = 65536;

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
void Populate (int n, struct list_node_s** head_pp, int *data);
double Execute (int n, int m, float memb, float ins, float del, struct list_node_s** head_pp, int *data);
void PrintStat (double *stat, int n_obs);

//----------------------------------------------------------------------------------------------------------------------------------------//
// Main method

int main(int argc, char* argv[]) {
	struct list_node_s** head; 
	
	/* 	n 	- number of random numbers
		m 	- number of operations
		memb	- member operation fraction 
		ins	- insert operation fraction
		del	- delete operation fracrion
		n_obs	- number of observations
	*/
	int n 		= strtol (argv[1], NULL, 10);
	int m 		= strtol (argv[2], NULL, 10);
	float memb 	= strtof (argv[3], NULL);
	float ins 	= strtof (argv[4], NULL);
	float del	= strtof (argv[5], NULL);
	int n_obs	= strtol (argv[6], NULL, 10);

 	printf ("Input Arguments n = %d m = %d memb = %f ins = %f del = %f \n",n,m,memb,ins,del);

	int count;
	int data[n+m];
	double stat[n_obs];

	for (count = 0; count < n_obs; count++) {
		head = malloc(sizeof(struct list_node_s));

		Random (data, n + m);

		Populate(n, head, data);
	
		stat[count] = Execute (n, m, memb, ins, del, head, data);
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
void Populate (int n, struct list_node_s** head_pp, int *data) {
	
	int i;

	for (i = 0; i < n; i++) {
		int p = Insert (data[i], head_pp);
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------//
// Execute

double Execute (int n, int m, float memb, float ins, float del, struct list_node_s** head_pp, int *data) {

	// Randomly execute the operations
	int member_ops = m * memb;
	int insert_ops = m * ins;
	int delete_ops = m * del;

	double start,end,elapsed_time;

	int i = n;

	GET_TIME(start);
	while (member_ops > 0 || insert_ops > 0 || delete_ops > 0) {
		int temp = rand() % 3;
		
		if (temp == 0 && member_ops > 0) { // Do member operation
			Member (data[i], *head_pp);
			member_ops--;
			i++;
		} else if (temp == 1 && insert_ops > 0) { // Do insert operation
			Insert (data[i], head_pp);
			insert_ops--;
			i++;
		} else if (temp == 2 && delete_ops > 0) { // Do delete operaion
			Delete (data[i], head_pp);
			delete_ops--;
			i++;
		} else {
			continue;
		}
	}
	GET_TIME(end);

	elapsed_time = end - start;

	return elapsed_time;
	
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





