/*************************************************************
 * Last Name:   Tashnim
 * First Name:  Ummey Zarin
 * Student ID:  30034931
 * Course:      CPSC 457
 * Tutorial:    03
 * Assignment:  5
 * Question:    6
 *
 * File name: pagesim.cpp
 *
 *************************************************************/
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

void Optimal(vector<int> ref_string, int total_frames);
void LRU(vector<int> ref_string, int total_frames);
void Clock(vector<int> ref_string, int total_frames);

struct page {
  int value;
  int ageOfValue;
};

void Optimal(vector<int> ref_string, int total_frames)
{
  vector<page*> frames;
  int page_faults;

  cout << "Optimal: \n";  
  int k=0, ageOfValue = 0, f=0;
  bool found = false;
  
  //while j < ref_string size, keep incrementing j
  for (int j=0; j < ref_string.size(); j++)
    {
      //while f < frame size and ref_string value is not in frame, keep incrementing f
      for (f=0; f < frames.size() && !(found = (frames[f]->value == ref_string[j])); f++);
      //if ref_string value not found
      if (!found)
	{
	  //increment page faults
	  page_faults++;
	  //while age of value < ref_string size && ref_string value at current
	  //pos is not equal to ref_string as next pos, keep incremnting the age
	  for (ageOfValue = j+1; ageOfValue < ref_string.size() && ref_string[ageOfValue] != ref_string[j]; ageOfValue++);
	  //if the frame is not filled yet
	  if (frames.size() < total_frames)
	    {
	      //push in the initial values
	      frames.push_back(new page);
	      //k = frame index you're looking at
	      k = frames.size()-1;
	    }
	  else
	    {
	      //while f < frame size, keep incrementing f
	      for (int f = 0, v = 0; f < frames.size(); f++)
		{
		  //if age of value of frame at current postion is > index v
		  if (frames[f]->ageOfValue > v)
		    {
		      k = f;                    //the frame index you're looking at
		      v = frames[k]->ageOfValue;//set v = age of value at current frame index
		    }
		}
	    }
	  //set current frame value = current ref_string
	  frames[k]->value = ref_string[j];
	  //set current frame value = age of value
	  frames[k]->ageOfValue = ageOfValue;
	}else{
	//while age of value < ref_string size && ref_string value at current
	//pos is not equal to ref_string as next pos, keep incremnting the age
	for (ageOfValue = j+1; ageOfValue < ref_string.size() && ref_string[ageOfValue] != ref_string[j]; ageOfValue++);
	frames[f]->ageOfValue = ageOfValue;
      }
    }

  cout << " - frames: ";
  //print the last values in frames
  for(int i = 0; i < total_frames; i++)
    cout << frames[i]->value << " ";
  cout << "\n - page faults: " << page_faults << "\n";
}

void LRU(vector<int> ref_string, int total_frames)
{
  //LRU - Least Recently Used
  vector<page*> frames;
  int page_faults;
  
  cout << "LRU: \n";  
  int ageOfValue = 0;
  int i = 0;
  bool found = false;

  //while j < ref_string size, keep incrementing j
  for (int j=0; j < ref_string.size(); j++)
    {
      //while f < frame size and ref_string value is not in frame, keep incrementing f
      found = false;
      for (int f=0; f < frames.size() && !(found = (frames[f]->value == ref_string[j])); f++);
      if (found == false && frames.size() < total_frames)
	{
	  page* p = new page;
	  p->value = ref_string[j];
	  frames.push_back(p);    //push in the initial values
	  found = true;                  //set found to true
	  page_faults++;                 //increment page faults
	}
      //if not found in frame
      int mx = 0, index = -1;
      if (!found)
	{
	  for (int k = 0; k < frames.size(); k++)
	    {
    //	      for (i = j-1, ageOfValue = 1; frames[k]->value != ref_string[i]; ageOfValue++, i--);
	      i = j - 1;
	      ageOfValue = 1;
	      while (frames[k]->value != ref_string[i])
		{
		  ageOfValue++;
		  i--;
		}
	      if (ageOfValue > mx)
		{		  
		  mx = ageOfValue;
		  index = k;
		}
	    }
	  frames[index]->value = ref_string[j];
	  page_faults++;
	}
    }
  cout << " - frames: ";
  //print the last values in frames
  for(int i = 0; i < frames.size(); i++)
    cout << frames[i]->value << " ";
  cout << "\n - page faults: " << page_faults << "\n";	
}

void Clock(vector<int> ref_string, int total_frames)
{
  //Clock Algorithm
  vector<int> frames;
  vector<int> bit;
  int page_faults;
  int clock = 0;
  int replace = -1;
  bool found = false;

  cout << "Clock: \n";  
  //while frame is not full, fill it with initial values
  for (int i=0; i < total_frames; i++)
    {
      if (std::find(frames.begin(), frames.end(), ref_string[i]) != frames.end())
	{
	  break;
	}
      else
	{
	  frames.push_back(ref_string[i]);
	  bit.push_back(1);
	  page_faults++;
	}
    }

  for(int i = total_frames; i < ref_string.size(); i++)
    {
      found = false;
      for(int j = 0; j < total_frames; j++)
	{
	  //if page is already inside a frame then break
	  if(frames[j] == ref_string[i])
	    {
	      found = true;
	      bit[j] = 1;
	      break;
	    }
	}
      //if page is not in frame, then replace page
      if(found == false)
	{
	  while(found == false)
	    {
	      //if bit is 0, replacement is found
	      if(bit[clock] == 0)
		{
		  //replace
		  replace = clock;
		  //make sure clock doesn't go out of frame bound
		  if(clock == total_frames-1)
		    clock = 0;
		  else
		    clock++;
		  break;
		}
	      else
		{
		  //else set bit to 0, we haven't used the page yet
		  bit[clock] = 0;
		  //make sure clock doesn't go out of frame bound
		  if(clock == total_frames-1)
		    clock = 0;
		  else
		    clock++;
		}
	    }
	  frames[replace] = ref_string[i];     //replace frame
	  bit[replace] = 1;                    //change bit to 1, we used the page
	  page_faults++;                       //increment page faults
	}
      /*        cout << "frames: ";
	for(int i = 0; i < total_frames; i++)
	  cout << frames[i] << " ";
	  cout << endl;*/
    }
  cout << " - frames: ";
  for(int i = 0; i < total_frames; i++)
    cout << frames[i] << " ";
  cout << "\n - page faults: " << page_faults << "\n";	
}

int main(int argc, char ** argv)
{
  vector<int> ref_string;
  int total_frames;
  
  if (argc < 2) {
    printf("Please put an argument: <number of available frame>\n");
    exit(-1);
  }
  total_frames = atoi(argv[1]);  //enter the avaliable frame
  int input;

  while (cin >> input)
    ref_string.push_back(input);
  /*
  cout << "Reference string: ";
  for (int i=0; i < ref_string.size(); i++)
    cout << ref_string[i] << " ";
  cout << endl;
  */
  Optimal(ref_string, total_frames);  //Optimal algorithm
  LRU(ref_string, total_frames);      //LRU algorithm
  Clock(ref_string, total_frames);    //Clock algorithm
  return 0;
}

