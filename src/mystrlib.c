/**
** \file mystrlib.c
** \brief Basic functions implementing operations on strings
** \author Maroua Mesbahi
** \version 1.0
** \date 12/11/2017
** mystrlib.c contains various operations on strings
** such as copying and concatenation
**
*/

#include <stdlib.h>
#include "mystrlib.h"


/**
** \brief Functions that calculate the length of a string
** \param String
** \return integer representing the length of the string
**/
size_t mystrlen(char *str)
{
  size_t res = 0;
  while(str[res] != '\0')
    res++;
  return res;
}

/**
** \brief fill the res argument
** \details fill the res argument with the right path
**     from the variable path to the variable dir_name
** \param res   the resulting string containing the right path at the end
** \param path   a string containing the path
** \param dir_name   the name of the file/directory that will be concatenated
*/
void make_path(char *res, char *path, char *dir_name)
{
  size_t len = mystrlen(path);
  size_t i = 0;
  size_t j = 0;
  while(i < len)
  {
    res[i] = path[i];
    i++;
  }
  if(res[i-1] != '/')
  {
    res[i] = '/';
    i++;
  }
  len = mystrlen(dir_name);
  while(j < len)
  {
    res[i] = dir_name[j];
    j++;
    i++;
  }
  res[i] = '\0';
}

/**
** \brief compare two strings
** \param str1   first string
** \param str2   second string
** \return 0 if str1 matches str2, 1 if str1 is greater than str2
** and -1 if str1 is less than str2
*/
int mystrcmp(char *str1, char *str2)
{
  size_t len1 = mystrlen(str1);
  size_t len2 = mystrlen(str2);
  size_t index = 0;
  if(len1 > len2)
    index = len2;
  else
    index = len1;
  size_t i = 0;
  for(i = 0; i < index; i++)
  {
    if(str1[i] > str2[i])
      return 1;
    else if(str1[i] < str2[i])
      return -1;
  }
  if(len1 == len2)
    return 0;
  else if(len1 > len2)
    return 1;
  return -1;
}

/**
** \brief copy dest in src
** \param src   the string to be copied
** \param dest   where to copy the string
** \param len   the length of dest
*/
void copy_str(char *src, char *dest, size_t len)
{
  size_t i = 0;
  len = len;
  size_t l = mystrlen(src);
  for(i = 0; i < l; i++)
    dest[i] = src[i];
  dest[l] = '\0';
}

/**
** \brief concatenate two string
** \param res   the resulting string
** \param str   the string which will me added to res
** \param b   the begging index of the copy
** \param res_len   the length of the resulting string
*/
int mystrcat(char *res, char *str, int b, int res_len)
{
  size_t len = mystrlen(str);
  size_t i = 0;
  int j = 0;
  while(i < len)
  {
    if(b+j > res_len)
      return 0;
    res[b+j] = str[i];
    j++;
    i++;
  }
  res[b+j] = '\0';
  return b+j;
}

/**
** \brief get the type of an expression
** \details the type would be either name, type, print or exec
** \param expr   the expression
** \return a string containing the type of the expression
*/
char *get_type(char *expr)
{

  size_t i = 0;
  if(mystrcmp(expr, "-print") == 0)
    return "-print";
  size_t len = mystrlen(expr);
  char *res = malloc(6);
  if(len > 6)
  {
    while(expr[i] != ' ')
    {
      res[i] = expr[i];
      i++;
    }
    res[i] = '\0';
  }
  return res;
}

/**
** \brief Check if a String contains a substring
** \param str   The principal string
** \param subs   The substring
** \return 0 if successs, 0 otherwise
*/
int str_contains(char *str, char *subs)
{
	size_t len = mystrlen(str);
	size_t i = 0;
	if(len < 6)
		return 1;
	for (i = 0; i < 5; i++)
	{
		if(str[i] != subs[i])
			return 1;
	}
	return 0;
}
