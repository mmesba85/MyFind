#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "myfind.h"
#include "mystrlib.h"


int myfind(char *dir_name, struct expressions_list *el, int is_d, int is_h, int is_l, int is_p)
{
  el = el;
  DIR *dir = NULL;
  if(dir_name)
    dir = opendir(dir_name);
  else
    opendir(".");
  if(!dir)
    return 1;
  
  struct dirent *readfile;
  if(is_h == 0)
  {
  	struct stat buff;
  	int st = lstat(dir_name, &buff);
  	if( st == -1)
  		return 1;
  	if(S_ISLNK(buff.st_mode) != 0)
  		return 1;
  }
  if(is_d == 0)
  	printf("%s\n", dir_name);
  while((readfile = readdir(dir)))
  {
    if(mystrcmp(readfile->d_name, ".") == 0 || mystrcmp(readfile->d_name, "..") == 0)
      continue;
    char dir_path[mystrlen(dir_name) + mystrlen (readfile->d_name) + 1];
    make_path(dir_path, dir_name, readfile->d_name);
    if(readfile->d_type == DT_DIR && mystrcmp(readfile->d_name, ".") != 0 
      && mystrcmp(readfile->d_name, "..") != 0)
    	myfind(dir_path, el, is_d, 0, is_l, is_p);
    else if(readfile->d_type == DT_LNK && is_l == 1)
    	myfind(dir_path, el, is_d, 1, is_l, is_p);
    else
    	printf("%s\n",dir_path); 
  }
  if(is_d == 1)
  	printf("%s\n",dir_name);
  closedir(dir);
  return 0;
}
