Arukgoda J.S.
100040X

CS 4532
Lab 1

1) llist_a.c 
	Serial Program
	To compile	: "gcc -o llist_a llist_a.c -lm"
	To run		: "./llist_a 1000 10000 0.99 0.005 0.005 15" where input parameters are n, m, memb, ins, del and n_obs 				respectively	
			/* 	n 	- number of random numbers
				m 	- number of operations
				memb	- member operation fraction 
				ins	- insert operation fraction
				del	- delete operation fracrion
				n_obs	- number of observations
			*/

2) llist_b.c
	Parallel Program with one mutex 
	To compile	: "gcc -o llist_b llist_b.c -lm -lpthread"
	To run		: "./llist_b 1000 10000 0.99 0.005 0.005 100 4" where input parameters are n, m, memb, ins, del n_obs and tc 				respectively 
			/* 	n 	- number of random numbers
				m 	- number of operations
				memb	- member operation fraction 
				ins	- insert operation fraction
				del	- delete operation fracrion
				n_obs	- number of observations
				tc	- thread count
			*/

3) llist_c.c
	Parallel Program with read-write block
	To compile	: "gcc -o llist_c llist_c.c -lm -lpthread"
	To run		: "./llist_c 1000 10000 0.99 0.005 0.005 200 4" where input parameters are n, m, memb, ins, del n_obs and tc 				respectively 
			/* 	n 	- number of random numbers
				m 	- number of operations
				memb	- member operation fraction 
				ins	- insert operation fraction
				del	- delete operation fracrion
				n_obs	- number of observations
				tc	- thread count
			*/
