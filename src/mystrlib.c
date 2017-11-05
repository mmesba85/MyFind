#include <stdlib.h>
#include "mystrlib.h"

size_t mystrlen(char *str)
{
	size_t res = 0;
	while(str[res] != '\0')
		res++;
	return res;
}

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

void copy_str(char *src, char *dest, size_t len)
{
  size_t i = 0;
  for(i = 0; i< len; i++)
    dest[i] = src[i];
  dest[len] = '\0';
}

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
	return j;
}

char *get_type(char *expr)
{
	size_t len = mystrlen(expr);
	char *res = malloc(6);
	size_t i = 0;
	if(mystrcmp(expr, "-print") == 0)
	{
		res = "-print";
		return res;
	}
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