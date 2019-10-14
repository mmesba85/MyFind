/**
** \file myfind.c
** \brief The main project file
** \author Maroua Mesbahi
** \version 1.0
** \date 12/11/2017
**  This file do operations on directory as checking if the option matches the file
**	and then calls the myfind function that will explore the right directories
*/

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fnmatch.h>
#include <err.h>
#include <string.h>
#include <fcntl.h>
#include "myfind.h"
#include "mystrlib.h"

/**
** \brief Check if the given file argument is a link
** \param dir_name 	The file name
** \param ic 	The info_command structure
** \return 0 on success, 1 otherwise
** \details Check if the given file argument is a link and then
** whether the option matches
*/
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

/**
** \brief Open the directory given
** \param dir_name 	The directory name
** \return Dir structure
*/
DIR *open_dir(char *dir_name)
{
  DIR *dir = NULL;
  if(dir_name)
  {
    int fd = open(dir_name, O_CLOEXEC);
    dir = fdopendir(fd);
  }
  if(!dir)
  {
    warnx("'%s': No such file or directory", dir_name);
    return NULL;
  }
  return dir;
}

/**
** \brief The main myfind function
** \param dir_name 	The directory name
** \param ic 	The info_command structure
** \return 0 on success, 1 otherwise
** \details This method explores the tree recursively while calling readdir
*/
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
  {
  	closedir(dir);
  	return 1;
  }
  struct dirent *readfile = NULL;
  while((readfile = readdir(dir)))
  {
    if(strcmp(readfile->d_name, ".") == 0 || strcmp(readfile->d_name, "..") == 0)
      continue;

    char dir_path[strlen(dir_name) + strlen (readfile->d_name) + 1];
    make_path(dir_path, dir_name, readfile->d_name);

    if(readfile->d_type == DT_DIR && strcmp(readfile->d_name, ".") != 0
      && strcmp(readfile->d_name, "..") != 0)
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
  {
  	free_ic(ic);
  	return 1;
  }
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
