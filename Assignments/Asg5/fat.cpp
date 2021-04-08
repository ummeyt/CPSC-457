/*************************************************************
 * Last Name:   Tashnim
 * First Name:  Ummey Zarin
 * Student ID:  30034931
 * Course:      CPSC 457
 * Tutorial:    03
 * Assignment:  5
 * Question:    7
 *
 * File name: fat.cpp
 *
 *************************************************************/
// CPSC457 Fall 2017, University of Calgary
// Skeleton C++ program for Q7 of Assignment 5.
//
// The program reads in the input, then calls the (wrongly implemented) checkConsistency()
// function, and finally formats the output.
//
// You only need to reimplement the checkConsistency() function.
//
// Author: Pavol Federl (pfederl@ucalgary.ca or federl@gmail.com)
// Date: November 29, 2017
// Version: 1

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <cmath>
using namespace std;
typedef std::string SS;
typedef std::vector<SS> VS;

struct DEntry {
  SS fname = SS( 4096, 0);
  int size = 0;
  int ind = 0;
  bool tooManyBlocks = false;
  bool tooFewBlocks = false;
  bool hasCycle = false;
  bool sharesBlocks = false;
};

static SS join( const VS & toks, const SS & sep) {
  SS res;
  bool first = true;
  for( auto & t : toks) { res += (first ? "" : sep) + t; first = false;}
  return res;
}

// re-implement this function
//
// Parameters:
//   blockSize - contains block size as read in from input
//   files - array containing the entries as read in from input
//   fat - array representing the FAT as read in from input
// Return value:
//   the function should return the number of free blocks
//   also, for ever entry in the files[] array, you need to set the appropriate flags:
//      i.e. tooManyBlocks, tooFewBlocks, hasCycle and sharesBlocks
int checkConsistency( int blockSize, std::vector<DEntry> & files, std::vector<int> & fat)
{
  vector<SS> v(fat.size());
  int curr_file = 1;  //start from the first file
  //go though the files
  
  for (int i=0; i < files.size(); i++)
    {
      int curr_num_blocks = 0; //the current num of blocks
      //calculating the num of blocks:
      int num_blocks = ceil((float)files[i].size / (float)blockSize);
      int current = files[i].ind;
      bool has_next = true;       //a flag to check if there is a next block

      //while there is a next block
      while (has_next == true)
	{
	  //if block value is -1 then there is no next so break
	  if (current == -1)
	    {
	      has_next = false;
	      break;
	    }	  
	  else
	    {
	      //check if there is a file name
	      if (v[current] == "\0") //"NULL" doesn't seem to work here
		//if no, then set the filename (A, B, or C)
		v[current] = files[i].fname;
	      //else if there is a certain file name
	      else if (v[current] == files[i].fname)
		{
		  //the file has a cycle so break
		  files[i].hasCycle = true;
		  break;
		}
	      else
		{
		  //else the file shares it's blocks with other files
		  files[i].sharesBlocks = true;
		  //go though the temp file
		  //using for (int j=0; j < temp_file.size(); j++) seems to slow it down a lot
		  //so I'm using auto from here on
		  for (auto &temp_file : files)
		  {
		      //if the temp file name is the same as current
		      if (temp_file.fname == v[current])
			{
			  //then they share blocks
			  temp_file.sharesBlocks = true;
			  //so set the filename to current file then break
			  v[current] = files[i].fname;
			  break;
			}
		    }	
		}
	    }
	  //set the current to the next value
	  current = fat[current];
	  //increment the current number of blocks
	  curr_num_blocks++;
	}

      //Check if there is too many or too few blocks
      //if the current number of blocks is greater than the calculated num of blocks
      if (curr_num_blocks > num_blocks)
	//set tooManyBlocks flag to true
	files[i].tooManyBlocks = true;
      //else if the current number of blocks is less than the calculated num of blocks
      else if (curr_num_blocks < num_blocks)
	//then set tooFrewBlocks to be true
	files[i].tooFewBlocks = true;
      //move to the next file
      curr_file++;
    }

  int free_blocks = 0;
  //go though the temp vector
  for(auto temp : v)
    {
      //if the temp is NULL then keep incrementing the num of free blocks
      if(temp == "\0")
	{
	  free_blocks++;
	  //	  cout << "free blocks: " << free_blocks << endl;
	}
    }
  //return the number of free blocks
  return free_blocks;
}

int main()
{
  try {
    // read in blockSize, nFiles, fatSize
    int blockSize, nFiles, fatSize;
    if( 3 != scanf( "%d %d %d", & blockSize, & nFiles, & fatSize))
      throw "cannot read blockSize, nFiles and fatSize";
    if( blockSize < 1 || blockSize > 1024) throw "bad block size";
    if( nFiles < 0 || nFiles > 50) throw "bad number of files";
    if( fatSize < 1 || fatSize > 200000) throw "bad FAT size";
    // read in the entries
    std::vector<DEntry> entries;
    for( int i = 0 ; i < nFiles ; i ++ ) {
      DEntry e;
      if( 3 != scanf( "%s %d %d", (char *) e.fname.c_str(), & e.ind, & e.size))
	throw "bad file entry";
      e.fname = e.fname.c_str();
      if( e.fname.size() < 1 || e.fname.size() > 16)
	throw "bad filename in file entry";
      if( e.ind < -1 || e.ind >= fatSize) throw "bad first block in fille entry";
      if( e.size < 0 || e.size > 1073741824) throw "bad file size in file entry";
      entries.push_back( e);
    }
    // read in the FAT
    std::vector<int> fat( fatSize);
    for( int i = 0 ; i < fatSize ; i ++ ) {
      if( 1 != scanf( "%d", & fat[i])) throw "could not read FAT entry";
      if( fat[i] < -1 || fat[i] >= fatSize) throw "bad FAT entry";
    }

    int nFreeBlocks = checkConsistency( blockSize, entries, fat);
    size_t maxflen = 0;
    for( auto & e : entries ) maxflen = std::max( maxflen, e.fname.size());
    SS fmt = "  %" + std::to_string( maxflen) + "s: %s\n";

    printf( "Issues with files:\n");
    for( auto & e : entries ) {
      VS issues;
      if( e.tooFewBlocks) issues.push_back( "not enough blocks");
      if( e.tooManyBlocks) issues.push_back( "too many blocks");
      if( e.hasCycle) issues.push_back( "contains cycle");
      if( e.sharesBlocks) issues.push_back( "shares blocks");
      printf( fmt.c_str(), e.fname.c_str(), join( issues, ", ").c_str());
    }
    printf( "Number of free blocks: %d\n", nFreeBlocks);

  }
  catch( const char * err) {
    fprintf( stderr, "Error: %s\n", err);
  }
  //    catch( ... ) {
  //        fprintf( stderr, "Errro: unknown.\n");
  //    }
  return 0;
}
