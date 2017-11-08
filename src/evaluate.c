/**
** \file evaluate.c
** \brief Evaluate an expressions list on a given file
** \author Maroua Mesbahi
** \version 1.0
** \date 12/11/2017
** This file contains methods that will check if a given file
** matches the specifications of an expressions list
**
*/

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


/**
** \brief Check if a file_name matches the pattern given 
** \param path 	The path of a file
** \param readfile 	The struct dirent
** \param expr 	the expression that needs to be verified
** \return 1 if it matches, otherwise 0
** \details path parameter is useful when checking the origin file
** and expr parameter has to be of type '-name'
*/
int check_name(char *path, struct dirent *readfile, char *expr)
{
	if(mystrlen(expr) < 6)
		return 2;
	if(readfile != NULL && fnmatch(expr+6, readfile->d_name, FNM_PATHNAME) == 0)
		return 1;
	else if(fnmatch(expr+6, path, FNM_NOESCAPE) == 0)
		return 1;
	return 0;
}

/**
** \brief check the directory type
** \param path 	the path of the directory
** \param type 	the type that needs to be checked
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
** \param path 	The path of the file
** \param readfile 	The struct dirent corresponding to the file
** \param expr 	the expression the needs to be checked
** \return 1 if it matches, 0 otherwise
*/
int check_type(char *path, struct dirent *readfile, char *expr)
{
	if(mystrlen(expr) < 6)
		return 0;
	if(readfile == NULL)
		return check_dir_type(path, expr[6]);
	else
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

/**
** \brief The main evaluate function
** \param path 	The path of the file
** \param readfile 	The struct dirent corresponding to the file
** \param expr 	The element of an expressions_list
** \return 1 if it matches, 0 otherwise
** \details This method use the expression given as argument to check
** the type, and according to the type check its argument
*/
int evaluate(char *path, struct dirent *readfile, char *expr)
{
	char *type = get_type(expr);
	if(mystrcmp(type, "-name") == 0)
	{
		free(type);
		return check_name(path, readfile, expr);
	}
	else if(mystrcmp(type, "-type") == 0)
	{
		free(type);
		return check_type(path, readfile, expr);
	}
	else if(mystrcmp(type, "-print") == 0)
	{
		free(type);
		return 1;
	}
	free(type);
	return 0;
}

/**
** \brief The main loop of checking the expressions_list
** \param path 	The path name
** \param readfile 	The struct dirent corresponding to the file	
** \param ic 	the struct info_command containing details 
** about the command line
** \return 1 if it matches, 0 otherwise
*/
int check_el(char *path, struct dirent *readfile, struct info_command *ic)
{
	struct expressions_list *el = copy_stack(ic->el);
	int res = 0;
	int first = 0;
	int current_op = 0;
	while(el->start)
	{
		char *token = pop(el);
		if(mystrcmp(token, "-o") == 0)
		{
			char *expr = pop(el);
			int aux = evaluate(path, readfile, expr);
			free(expr);
			res = (aux + res) - (aux * res);
			first++;
			current_op = 0;
		}
		else if(mystrcmp(token, "-a") == 0)
		{
			if(first == 0)
			{
				res = 1;
				first++;
			}
			char *expr = pop(el);
			int aux = evaluate(path, readfile, expr);
			free(expr);
			res *= aux;
			current_op = 1;
		}
		else
		{
			int aux = evaluate(path, readfile, token);
			if(current_op == 0)
				res = (aux + res) - (aux * res);
			else
				res *= aux;
		}
		free(token);
	}
	if(res == 1)
		printf("%s\n", path);
	free_path(el);
	return res;
}