#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fnmatch.h>
#include <err.h>
#include "myfind.h"
#include "mystrlib.h"

int check_link(char *dir_name, struct info_command *ic)
{
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
  return 0;
}

DIR *open_dir(char *dir_name)
{
  DIR *dir = NULL;
  if(dir_name)
    dir = opendir(dir_name);
  else
    opendir(".");
  if(!dir)
  {
    warnx("'%s': No such file or directory", dir_name);
    return NULL;
  }
  return dir; 
}

int myfind(char *dir_name, struct info_command *ic)
{
  DIR *dir = open_dir(dir_name);
  if(dir == NULL)
    return 1;
  if(ic->opt_d == OPT_DEFAULT)
  {
    if(ic->el == NULL)
      printf("%s\n", dir_name);
    else
      check_el(dir_name, NULL, ic); 
  }
  int check = check_link(dir_name, ic);
  if(check != 0)
    return 1;
  struct dirent *readfile = NULL;
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

int main(int argc, char **argv)
{
  struct info_command *ic = initialize_ic();
  if(ic == NULL)
  	return 1;
  int parse = myparser(argc, argv, ic);
  if(parse != 0)
  	return 1;
  int i = 0;
  int res = 0;
  while(i < ic->nb_files)
  {
  	if(res == 1)
  		myfind(ic->files[i], ic);
  	else
  		res = myfind(ic->files[i], ic);
  	i++;
  }
  if(ic->nb_files == 0)
  	myfind(".", ic);
  free_ic(ic);
  return res;
}