#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fnmatch.h>
#include "myfind.h"
#include "mystrlib.h"

int myfind(char *dir_name, struct info_command *ic)
{
  DIR *dir = NULL;
  if(dir_name)
    dir = opendir(dir_name);
  else
    opendir(".");
  if(!dir)
    return 1;
  
  struct dirent *readfile;
  struct stat buff;
  int st = lstat(dir_name, &buff);
  if( st == -1)
  	return 1;
  if(S_ISLNK(buff.st_mode) != 0)
  {
  	if(ic->opt == OPT_H)
  		ic->opt = OPT_DEFAULT;
  	else if(ic->opt != OPT_L)
  		return 1;
  }
  
  if(ic->opt_d == OPT_DEFAULT)
  {
  	if(ic->el == NULL)
    	printf("%s\n", dir_name);
    else
    	check_el(dir_name, NULL, ic); 
  }

  while((readfile = readdir(dir)))
  {
    if(mystrcmp(readfile->d_name, ".") == 0 || mystrcmp(readfile->d_name, "..") == 0)
      continue;
    
    char dir_path[mystrlen(dir_name) + mystrlen (readfile->d_name) + 1];
    make_path(dir_path, dir_name, readfile->d_name);

    if(readfile->d_type == DT_DIR && mystrcmp(readfile->d_name, ".") != 0 
      && mystrcmp(readfile->d_name, "..") != 0)
    	myfind(dir_path, ic);
    else if(readfile->d_type == DT_LNK && ic->opt == OPT_L)
    	myfind(dir_path, ic);
   	else
    {
    	if(ic->el == NULL)
    		printf("%s\n", dir_path);
    	else
    		check_el(dir_path, readfile, ic); 
  	}
  }
  if(ic->opt_d == OPT_D)
  {
  	if(ic->el == NULL)
    	printf("%s\n", dir_name);
    else
    	check_el(dir_name, readfile, ic); 
  }
  closedir(dir);
  return 0;
}
