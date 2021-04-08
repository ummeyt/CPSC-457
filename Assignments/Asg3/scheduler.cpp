/**********************************************
 * Last Name:   Tashnim
 * First Name:  Ummey Zarin
 * Student ID:  30034931
 * Course:      CPSC 457
 * Tutorial:    03
 * Assignment:  3
 * Question:    7
 *
 * File name: scheduler.cpp
 *********************************************/
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <pthread.h>
#include <queue>
using namespace std;

typedef struct {
  int arrival[], burst[];
  int processNum;
  int processID;
} t_data;

//global variables
vector<int> numbers;  
queue<int> readyQueue;

//functions
int roundRobin(t_data* data, int rem_burst[], int waitTime[], int timeQuan, int t, bool done);
int shortestJobFirst(t_data* data, int rem_burst[], int waitTime[], int time, bool done);

int main (int argc, char * argv[])
{
  // handle command line arguments  
  string filename;
  if (argc < 4) {
    printf("Please put three arguments: <file name> <RR or SJF> <time quantum>\n");
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
      t_data data[4];
      int time = 0;
      for (int i = 0; infile >> curr_num; i++)
	{
	  numbers.push_back(curr_num);	  
	  if (i % 2 == 0) //even index - arrival time in file
	    {
	      data->arrival[i] = curr_num;
	    }
	  else            //odd index - burst time in file
	    {
	      data->burst[i] = curr_num;
	      time += data->burst[i];
	    }
	}
      infile.close();
      
      if (numbers.empty())
	cout << "File is empty" << endl;

      string choice = argv[2];
      int timeQuan = atoi(argv[3]);
      int rem_burst[100], waitTime[100];
      bool done = true;
      if (choice == "RR" || choice == "rr")
	{
	  roundRobin(data, rem_burst, waitTime, timeQuan, time, done);
	}
      else if (choice == "SJF" || choice == "sjf")
	{
	  shortestJobFirst(data, rem_burst, waitTime, time, done);
	}      
      else
	cout << "Error - invalid option. Please choose RR/rr or SJF/sjf.\n";
    }
  
  return 0;
}

int roundRobin(t_data* data, int rem_burst[], int waitTime[], int timeQuan, int time, bool done)
{
  //determine the number of processes
  data->processNum = numbers.size()/2;
  cout << "Time\t";
  int t = 0;
  while (done)
    {
      done = false;
      for (int i=0; i < data->processNum; i++)	
	{
	  rem_burst[i] = data->burst[i];
	  cout << "P"<< i <<"\t";        //print the processes
	  if (rem_burst[i] > 0)
	    {
	      done = true;
	      if (rem_burst[i] > timeQuan)
		{
		  t += timeQuan;    //incease value of time
		  rem_burst[i] -= timeQuan; //decrease burst time of current process
		}
	      else
		{
		  t = t + rem_burst[i];  //increase value of time
		  waitTime[i] = t - data->burst[i]; //calculate waiting time - time - burst time
		  rem_burst[i] = 0; //make remaining burst time 0
		}
	    }
	}
      cout << "\n---------------------------------------------------\n";

      //      int passTime = min(timeQuan, rem_burst[i]); 
      //print out the time - total burst time
      for (int j = 0; j <= time; j++)
	{
	  cout << j;
	  if (data->arrival[j] <= time)     //if the arrive <= time
	    {
	      cout << "\t.\n";              //then print it's running
	    }
	  else if (rem_burst[j] == 0)	    //if there is no burst time left
	    cout << "\t \n";                //then print it's waiting or done
	  else
	    cout << "\t+\n";                //then print it's ready
	}
      if (done)
	break;
    }
  float avgWait = 0;
  for (int i=0; i < data->processNum; i++)	
    {
      rem_burst[i] = data->burst[i];
      cout << "P" << i << " waited " << waitTime[i] << " secs\n";
      avgWait += waitTime[i];
    }
  cout << "Average waiting time = " << avgWait/data->processNum << " secs.\n";

  return 0;

}

int shortestJobFirst(t_data* data, int rem_burst[], int waitTime[], int time, bool done)
{
  int t = 0;
  //determine the number of processes
  data->processNum = numbers.size()/2;
  cout << "Time\t"; 
  while (done)
    {
      for (int i=0; i < data->processNum; i++)	
	{
	  rem_burst[i] = data->burst[i];
	  cout << "P"<< i <<"\t";
	  t = t + rem_burst[i];  //increase value of time
	  waitTime[i] = t - data->burst[i]; //calculate waiting time = time - burst time
	  rem_burst[i] = 0; //make remaining burst time 0
	}
      cout << "\n---------------------------------------------------\n";

      //print out the time - total burst time
      for (int j = 0; j <= time; j++)
	{
	  cout << j;
	  if (data->arrival[j] <= time)     //if the arrive <= time
	    {
	      cout << "\t.\n";              //then print it's running
	    }
	  else if (rem_burst[j] == 0)	    //if there is no burst time left
	    cout << "\t \n";                //then print it's waiting or done
	  else
	    cout << "\t+\n";                //then print it's ready
	}
      if (done)
	break;
    }
  float avgWait = 0;
  for (int i=0; i < data->processNum; i++)	
    {
      rem_burst[i] = data->burst[i];
      cout << "P" << i << " waited " << waitTime[i] << " secs\n";
      avgWait += waitTime[i];
    }
  cout << "Average waiting time = " << avgWait/data->processNum << " secs.\n";

  return 0;
}
