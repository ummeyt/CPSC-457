/**********************************************
 * Last Name:   Tashnim
 * First Name:  Ummey Zarin
 * Student ID:  30034931
 * Course:      CPSC 457
 * Tutorial:    03
 * Assignment:  2
 * Question:    5
 *
 * File name: sum.cpp
 *********************************************/
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <pthread.h>
using namespace std;

vector<int> numbers;  //global vector pratial value

typedef struct {
  int start, end;
  int id;
  int partSum;
} t_data;

void* start_func(void *arg) {

  t_data *data = (t_data*) arg;
  int sum = 0;

  for (unsigned int i=data->start; i<data->end; i++)	    
    sum += numbers[i];
  data->partSum = sum;
  pthread_exit(0);

  return NULL;
}

int main (int argc, char * argv[])
{
  // handle command line arguments  
  string filename;
  if (argc != 3) {
    printf("Please put two arguments: <file name> <number of threads>\n");
    exit (-1);
  }
  else
    filename = argv[1];

  // open the file
  ifstream infile(filename); 
  if(!(infile.is_open() && infile.good())) {
    cerr << "Could not open file " << filename << "\n";
    exit(-1);
  }
  else
    {
      // get the numbers from file
      int curr_num = 0;
      while (infile >> curr_num)
	numbers.push_back(curr_num);
      infile.close();

      int T = atoi(argv[2]);  //static cast the 2nd parameter into an int
      int N = numbers.size(); //vector size
      t_data data[T];
      pthread_t sum_thread[N];

      if (numbers.empty())
	cout << "File is empty" << endl;
      // if the num of threads is greater than numbers that exist
      else if(T > N)
	cout << "Number of threads cannot exceed the array size" << endl; 
      // else print the sums
      else
	{
	  // calculating the sums
	  data->start = N/T+1;
	  data->end = N/T; 
	  int sum = 0;
	  int rem = N%T;
	  int index = 0;
	  for (int i=0; i<T; i++) {
	    data[i].id = i+1;
	    data[i].start = index;
	    data[i].end = index+N/T;
	    if (rem > 0)
	      {
		data[i].end++;
		rem--;
	      }
	    index = data[i].end;
	    pthread_create(&sum_thread[i], NULL, start_func, &data[i]);	    
	  }
	  cout << endl;

	  // wait for all the threads
	  for (int i=0; i<T; i++)
	    pthread_join(sum_thread[i], NULL);

	  for (int i =0; i<T; i++) {
	    cout << "Thread " << data[i].id << ": " << data[i].partSum << endl;
	    sum += data[i].partSum;
	  }
	  cout << "sum = " << sum << endl;
	}   
    }
  
  return 0;
}
