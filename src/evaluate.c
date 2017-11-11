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
#include <errno.h>
#include "myfind.h"
#include "mystrlib.h"

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
  else if(is_source(path, ic->files, ic->nb_files) == 0 && fnmatch(expr+6, path, FNM_NOESCAPE) == 0)
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

/**
** \brief The main evaluate function
** \param path   The path of the file
** \param readfile   The struct dirent corresponding to the file
** \param expr   The element of an expressions_list
** \return 1 if it matches, 0 otherwise
** \details This method use the expression given as argument to check
** the type, and according to the type check its argument
*/
int evaluate(char *path, struct dirent *readfile, char *expr, struct info_command *ic, struct evaluate *e)
{
  char *type = get_type(expr);
  ic = ic;
  if(mystrcmp(type, "-name") == 0)
  {
    free(type);
    return check_name(path, readfile, expr, ic);
  }
  else if(mystrcmp(type, "-type") == 0)
  {
    free(type);
    return check_type(path, readfile, expr);
  }
  else if(mystrcmp(type, "-print") == 0)
  {
    e->nb_print += 1;
    return 1;
  }
  free(type);
  return 0;
}

char *eval_to_str(int i)
{
  char *str = malloc(2);
  if(i == 0)
  {
    str[0] = '0';
    str[1] = '\0';
  }
  else
  {
    str[0] = '1';
    str[1] = '\0';
  }
  return str;
}

int compare_boolean(int eval1, int eval2, int op)
{
  if(op == 0)
    return (eval1 || eval2);
  else
    return (eval1 && eval2);
}

int if_print(struct expressions_list *el)
{
	if (stack_contains(el, "-print") == 0)
    return PRINT_NODEF;
  else
	{
		if(stack_contains(el, "-name") == 0 || stack_contains(el, "-type") == 0)
      return PRINT_DEF;
	}
	return PRINT_NODEF;
}



struct evaluate *initialize_evaluate(struct expressions_list *el)
{
  struct evaluate *res = malloc(sizeof(struct evaluate));
  if(if_print(el) == PRINT_NODEF)
		res->nb_print = 0;
	else
		res->nb_print = 1;
  res->current_op = 2;
  return res;
}

void print_result(char *path, struct dirent *readfile, struct info_command *ic,
  struct evaluate *e, char *last)
{
  int evaluation_result = 0;
  if(mystrcmp(last, "1") != 0 && mystrcmp(last, "0") != 0)
    evaluation_result = evaluate(path, readfile, last, ic, e);
  else
  {
    if(mystrcmp(last, "1") == 0)
      evaluation_result = 1;
    else
      evaluation_result = 0;
  }
  if(evaluation_result == 1)
  {
    for(int i = 0; i < e->nb_print; i++)
      printf("%s\n", path);
  }
}

/**
** \brief The main loop of checking the expressions_list
** \param path   The path name
** \param readfile   The struct dirent corresponding to the file
** \param ic   the struct info_command containing details
** about the command line
** \return 1 if it matches, 0 otherwise
*/
int check_el(char *path, struct dirent *readfile, struct info_command *ic)
{
  struct expressions_list *el = reverse_stack(ic->el);
  struct expressions_list *res = initialize_stack();
  struct evaluate *e = initialize_evaluate(ic->el);
  int step = 0;
  int stop = 0;
  int eval = 0;
  while(el->start)
  {
    char *token = pop(el);
    if(mystrcmp(token, "-o") != 0 && mystrcmp(token, "-a") != 0)
    {
      push(res, token);
      free(token);
      continue;
    }
    if(mystrcmp(token, "-o") == 0)
      e->current_op = 0;
    else if(mystrcmp(token, "-a") == 0)
      e->current_op = 1;
    if(e->current_op != 2)
    {
      while(stop == 0 && res->start != NULL &&
        mystrcmp(res->start->data, "1") != 0 &&
        mystrcmp(res->start->data, "0") != 0)
      {
        char *expr = pop(res);
        int expr_eval = evaluate(path, readfile, expr, ic, e);
        if(step != 0)
          eval = compare_boolean(expr_eval, eval, e->current_op);
        else
          eval = expr_eval;
        step++;
        free(expr);
        if(e->current_op == 0 && eval == 1)
          stop = 1;
      }
      char *data = eval_to_str(eval);
      push(res, data);
      free(data);
      if(stop == 1)
      {
        free(token);
        break;
      }
    }
    free(token);
  }
  char *last = pop(res);
  print_result(path, readfile, ic, e, last);
  free(last);
  free(e);
  free_exprlist(res);
  free_exprlist(el);
  return 0;
}
