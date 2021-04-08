/****************************************************************
 * Last Name:   Tashnim
 * First Name:  Ummey Zarin
 * Student ID:  30034931
 * Course:      CPSC 457
 * Tutorial:    03
 * Assignment:  2
 * Question:    2
 *
 * File name: myFind.c
 * 
 * NOTE: The following code has been copied directed and has been
 *       optimized from "recursive_ls.c"
 *****************************************************************/

#include <stdio.h>
#include <dirent.h>
#include <string.h>

void visitDir(const char *path) {
  DIR *directory = opendir(path);
  struct dirent* element = NULL;
  char buffer[1000];
    
  if(!directory) return;

  while(element = readdir(directory))
    {
      if (element->d_type != DT_DIR)
	printf("%s/%s\n", path, element->d_name);
      if(element->d_type == DT_DIR && strcmp(element->d_name, ".") && strcmp(element->d_name, ".."))
	{
	  sprintf(buffer, "%s/%s", path, element->d_name);
	  visitDir(buffer);
	}
    }
}

int main()
{
  visitDir(".");
  return 0;
}
