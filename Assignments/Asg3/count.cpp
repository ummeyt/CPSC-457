/**********************************************
 * Last Name:   Tashnim
 * First Name:  Ummey Zarin
 * Student ID:  30034931
 * Course:      CPSC 457
 * Tutorial:    03
 * Assignment:  3
 * Question:    5
 *
 * File name: count.cpp
 *********************************************/
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
using namespace std;

typedef struct{
  int64_t start;
  bool prime = true;
} data;

int isPrime(int64_t n);
int nThreads;
pthread_t *threads;
int64_t num;

void* thread_func(void *arg)
{
  data* d = (data*)arg;

  if (!isPrime(d->start))
    {
      d->prime = false;
    }

  return NULL;
}

int isPrime(int64_t start)
{
  if( num <= 1)
    {
      return 0; // small numbers are not primes
    }
  if( num <= 3)
      return 1; // 2 and 3 are prime
  if( num % 2 == 0 || num % 3 == 0)
      return 0; // multiples of 2 and 3 are not primes

  int64_t i = start;
  int64_t max = sqrt(num);
  while( i <= max) {
    if (num % i == 0 || num % (i+2) == 0)
      return 0;         
    i += (6 * nThreads);
  }
  return 1;
}

int main(int argc, char **argv) {
  if(argc < 2) {
    fprintf(stderr, "Number of threads is needed\n");
    exit(1);
  }

  nThreads = atoi(argv[1]);
  if( nThreads < 1 || nThreads > 256) {
    printf("Bad arguments. 1 <= nThreads <= 256!\n");
  }

  printf("Counting primes using %d thread%s.\n",
	 nThreads, nThreads == 1 ? "s" : "");

  threads = new pthread_t[nThreads];

  int count = 0;
  data params[nThreads];
  // 
  while(1) {
    int readOne = scanf("%ld", & num);    //read one number at a time
    if(readOne != 1)                      //if is not one then break
      break;
    else
      {
	int64_t temp = 5;                       //initial value
	for (int i = 0; i < nThreads; i++)
	  {
	    /*
	      if(num <= 10000)
	      {
	         if (isPrime(num))
	         count++;
	      }
	    */
	    params[i].prime = true;
		params[i].start = temp;
		temp += 6;
		pthread_create(&threads[i], NULL, thread_func, &params[i]);  //create threads
	  }
	// wait for all the threads
	for (int i=0; i<nThreads; i++)
	  pthread_join(threads[i], NULL);

	bool success = true;
	for (int i=0; i<nThreads; i++)
	  {
	    if (params[i].prime == false)
	      success = false;	    
	  }
	if (success)
	  count++;
      }
    
  }

  printf("Found %ld primes.\n", count);
  
  delete threads;
  return 0; 
}

