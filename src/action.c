/**
** \file action.c
** \brief Process actions
** \author Maroua Mesbahi
** \version 1.0
** \date 12/11/2017
**  Executes the command given to '-exec'
*/

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fnmatch.h>
#include <string.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include "myfind.h"
#include "mystrlib.h"


/**
** \brief Counts the number of arguments given to '-exec'
** \param type  The type of the execution command
** \param expr  The expression
** \return The number of arguments
** \details The type would be either exec of execidr
*/
int nb_args(char *type, char *expr)
{
  size_t len = mystrlen(expr);
  int res = 1;
  size_t i = 0;
  if(mystrcmp(type, "-exec") == 0)
    i = 6;
  else
    i = 9;
  while(i < len)
  {
    if(expr[i] == ' ')
      res++;
    i++;
  }
  return res;
}

/**
** \brief Count the length of an argument
** \param expr  The expression
** \param start   The index
** \param len   The length of the expression
** \return The length of an argument
*/
size_t get_arg_len(char *expr, size_t start, size_t len)
{
  while(start < len && expr[start] != ' ')
    start++;
  return start;
}

/**
** \brief Create an array containing the command and its arguments
** \param type  The type of execution
** \param expr  The expression
** \return An array containing the command and its arguments
*/
char **get_args(char *type, char *expr)
{
  size_t len = mystrlen(expr);
  int nb = nb_args(type, expr);
  int j = 0;
  int i = 0;
  size_t index = 0;
  if(mystrcmp(type, "-exec") == 0)
    index = 6;
  else
    index = 9;
  char **arguments = NULL;
  if(nb > 0)
  {
    arguments = malloc((nb+1) * sizeof(char *));
    if(!arguments)
      return NULL;
  }
  while(index < len)
  {
    size_t l = get_arg_len(expr, index, len);
    arguments[j] = calloc(l+1, sizeof(char));
    while(index < len && expr[index] != ' ')
    {
      arguments[j][i] = expr[index];
      i++;
      index++;
    }
    index++;
    arguments[j][i] = '\0';
    i = 0;
    j++;
  }
  arguments[j] = NULL;
  return arguments;
}

/**
** \brief Frees the memory allocated for the arguments array
** \param args  The arguments array
** \param nb  The length of the array
*/
void free_args(char **args, int nb)
{
  int i = 0;
  for (i = 0; i < nb; i++)
    free(args[i]);
  free(args);
}

/**
** \brief The main execution function
** \param type  The type of execution
** \param expr  The expression
** \details This function use an array containing the command and its arguments,
** then it call execvp
*/
int exec_command(char *type, char *expr)
{
  char **args = get_args(type, expr);
  int nb = nb_args(type, expr);
  pid_t pid = fork();

  if(pid == 0)
  {
    if(mystrcmp(type, "-exec") == 0)
      execvp(args[0], args);
    exit(1);
  }
  else
  {
    int status = 0;
    waitpid(pid, &status, 0);
    if(WIFEXITED(status) && WEXITSTATUS(status) != 0)
    {
      free_args(args, nb);
      free(type);
      warnx("cannot do %s: %s", expr, strerror(WEXITSTATUS(status)));
      return 0;
    }
    free_args(args, nb);
    free(type);
  }
  return 1;
}
