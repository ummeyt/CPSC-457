/***********************************************************************
 * Last Name:   Tashnim
 * First Name:  Ummey Zarin
 * Student ID:  30034931
 * Course:      CPSC 457
 * Tutorial:    03
 * Assignment:  2
 * Question:    3
 *
 * File name: scan.c
 *
 * NOTE: Some of the code has been copied directly from "c_struct_sort.c"
 ***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_FNAME_SIZE 512 //the max pathname length of any file will be < 512
#define MAX_FILES 1024     //the max num of files in directory will be < 1000


//Definition of the file structure
typedef struct {
  char file_name[MAX_FNAME_SIZE];
  int file_size;
} file_info;

//If f1 should be before f2, this function should return negative value
//So, file with a bigger size will be before the others
int comp_files(const void *f1, const void *f2) {
  return ((file_info*)f2)->file_size - ((file_info*)f1)->file_size;
}

int main (int argc, char ** argv) {
  // handle command line arguments  
  if (argc != 3) {
    printf("Please put two arguments: <suffix of files> <num of files>\n");
    exit (-1);
  }
  
  // open 'find...'
  FILE * fp = popen( "find . -type f", "r");
  if( fp == NULL) {
    perror("popen failed :(\n");
    exit(-1);
  }
 
  // read in all filenames
  char buff[MAX_FNAME_SIZE];
  int nFiles = 0;
  char * files[MAX_FILES];
  
  while(fgets(buff,MAX_FNAME_SIZE,fp)) {
    int len = strlen(buff) - 1;
    files[nFiles] = strndup(buff,len);
    nFiles ++;
  }

  file_info fileInfo [MAX_FILES];
  int c = 0;
  int len = strlen(argv[1]); 
  // filtering filenames based on extension
  for(int index = 0 ; index < nFiles ; index++) {
    int len2 = strlen(files[index]);
    if (len2 >= len && strcmp(files[index] + len2 - len, argv[1]) == 0) {
      strcpy(fileInfo[c].file_name, files[index]);
      struct stat myst;
      if( 0 != stat(fileInfo[c].file_name,&myst)) {
	perror("my stat failed :(\n");
	exit(-1);
      }
      fileInfo[c].file_size = myst.st_size;
      c++;
    }
  }
  fclose(fp);
  
  long long totalSize = 0;
  struct stat st;
  
  for(int i = 0 ; i < nFiles ; i++) {
    if( 0 != stat(files[i],&st)) {
      perror("stat failed :(\n");
      exit(-1);
    }
  }

  int count = 0;
  char * tempcharptr;
  int argv2 = strtol(argv[2], &tempcharptr, 10);    //number of files to output
  printf("\n");
  //Sort the array using the comparison function
  qsort(fileInfo, nFiles, sizeof(file_info), comp_files);

  while (strcmp(fileInfo[count].file_name, "") != 0 && count < argv2)
    {
      totalSize += fileInfo[count].file_size;       //calculating the sum
      printf("%s %ld\n", fileInfo[count].file_name, fileInfo[count].file_size);
      count++;
    }
  printf("\nTotal size: %lld\n\n", totalSize);

  // clean up
  for(int i = 0; i < nFiles ; i ++ ) {
    free(files[i]);
  }
  // return success
  return 0;
}
