/**
** \file test.c
** \brief Evaluate an expressions list on a given file
** \author Maroua Mesbahi
** \version 1.0
** \date 12/11/2017
** This file contains methods that will check if a given file
** matches the specifications of an test expression 
*/

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fnmatch.h>
#include <errno.h>
#include "myfind.h"
#include "mystrlib.h"

/**
** \brief Check if the file given to check_name is a file given as argument
** \param path   The file name to be checked
** \param files   Array containing files names given as argument
** \param nb   Number of files given as argument
*/
int is_source(char *path, char **files, int nb)
{
  for(int i = 0; i < nb; i++)
  {
    if(mystrcmp(files[i], path) == 0)
      return 0;
  }
  return 1;
}

/**
** \brief Get the directory name from a path
** \param path  The pwd of the directory
** \return The actual directory name
*/
char *get_dir(char *path)
{
  int aux = 0;
  int index = 0;
  int count = 0;
  int len = mystrlen(path);
  while(index < len)
  {
    if(path[index] == '/')
      aux++;
    index++;
  }
  index = 0;
  while(index < len)
  {
    if(path[index] == '/')
      count++;
    if(count == aux)
      break;
    index++;
  }
  return path+index+1;
}

/**
** \brief Check if a file_name matches the pattern given
** \param path   The path of a file
** \param readfile   The struct dirent
** \param expr   the expression that needs to be verified
** \return 1 if it matches, otherwise 0
** \details path parameter is useful when checking the origin file
** and expr parameter has to be of type '-name'
*/
int check_name(char *path, struct dirent *readfile, char *expr, struct info_command *ic)
{
  if(mystrlen(expr) < 6)
    return 2;
  if(readfile != NULL)
  {
    if(fnmatch(expr+6, readfile->d_name, FNM_PATHNAME) == 0)
      return 1;
    return 0;
  }
  ic = ic;
  char *dir_name = get_dir(path);
  if(fnmatch(expr+6, dir_name, FNM_PATHNAME) == 0)
    return 1;
  return 0;
}

/**
** \brief check the directory type
** \param path   the path of the directory
** \param type   the type that needs to be checked
** \return 1 if it matches, otherwise 0
*/
int check_dir_type(char *path, char type)
{

  struct stat buff;
  int st = stat(path, &buff);
  if( st == -1)
      return 0;
    switch(type)
    {
      case 'b':
      if(S_ISBLK(buff.st_mode) != 0)
        return 1;
      break;
    case 'c':
      if(S_ISCHR(buff.st_mode) != 0)
        return 1;
      break;
    case 'd':
      if(S_ISDIR(buff.st_mode) != 0)
        return 1;

      break;
    case 'f':
      if(S_ISREG(buff.st_mode) != 0)
        return 1;
      break;
    case 'l':
      if(S_ISLNK(buff.st_mode) != 0)
        return 1;
      break;
    case 'p':
      if(S_ISFIFO(buff.st_mode) != 0)
        return 1;
      break;
    case 's':
      if(S_ISSOCK(buff.st_mode) != 0)
        return 1;
      break;
    default:
      break;

    }
    return 0;
}

/**
** \brief check The type of a given file
** \param path   The path of the file
** \param readfile   The struct dirent corresponding to the file
** \param expr   the expression the needs to be checked
** \return 1 if it matches, 0 otherwise
*/
int check_type(char *path, struct dirent *readfile, char *expr)
{

  if(mystrlen(expr) < 6)
    return 0;
  if(readfile == NULL)
    return check_dir_type(path, expr[6]);

  if(readfile != NULL)
  {
    switch(expr[6])
    {
      case 'b':
        if(readfile->d_type == DT_BLK)
          return 1;
        break;
      case 'c':
        if(readfile->d_type == DT_CHR)
          return 1;
        break;
      case 'd':
        if(readfile->d_type == DT_DIR)
          return 1;
        break;
      case 'f':
        if(readfile->d_type == DT_REG)
          return 1;
        break;
      case 'l':
        if(readfile->d_type == DT_LNK)
          return 1;
        break;
      case 'p':
        if(readfile->d_type == DT_FIFO)
          return 1;
        break;
      case 's':
        if(readfile->d_type == DT_SOCK)
          return 1;
        break;
      default:
        break;
    }
  }
  return 0;
}
