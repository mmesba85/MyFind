/**
** \file evaluate.c
** \brief Evaluate an expressions list on a given file
** \author Maroua Mesbahi
** \version 1.0
** \date 12/11/2017
** This file contains methods that would go through the stack and call the right
** function to either do a test or execute an action
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
#include <string.h>
#include "myfind.h"
#include "mystrlib.h"

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
  if(strcmp(type, "-name") == 0)
  {
    free(type);
    return check_name(path, readfile, expr, ic);
  }
  else if(strcmp(type, "-type") == 0)
  {
    free(type);
    return check_type(path, readfile, expr);
  }
  else if(strcmp(type, "-print") == 0)
  {
    e->nb_print += 1;
    return 1;
  }
  else if(strcmp(type, "-exec") == 0 || strcmp(type, "-execdir") == 0)
  {
    if(e->eval == 1 || e->step == 0)
      return exec_command(type, expr);
    return 0;
  }
  free(type);
  return 0;
}

/**
** \brief Converts the evaluation results integer into a string
** \param eval  The evaluation result
** \return The string containing the evaluation result
*/
char *eval_to_str(int eval)
{
  char *str = malloc(2);
  if(eval == 0)
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

/**
** \brief Compares two booleans using an operator
** \param eval1   First boolean
** \param eval2   Second boolean
** \param op   The operator used to compare
** \return The result of the comparison
*/
int compare_boolean(int eval1, int eval2, int op)
{
  if(op == 0)
    return (eval1 || eval2);
  else
    return (eval1 && eval2);
}

/**
** \brief Check whether the command line contains a print action
** \param el  The expressions_list
** \return PRINT_NODEF if the print action is set, PRINT_DEF if it contains
** a test command and no print action
*/
int if_print(struct expressions_list *el)
{
	if (stack_contains(el, "-print") == 0)
    return PRINT_NODEF;
  else
	{
    if(stack_contains(el, "-exec") == 0 || stack_contains(el, "-execdir") == 0)
      return PRINT_NODEF;
    if(stack_contains(el, "-name") == 0 || stack_contains(el, "-type") == 0)
        return PRINT_DEF;
  }
	return PRINT_NODEF;
}

/**
** \brief initialize an evaluate structure
** \param el  The expressions_list
** \return struct evaluate
*/
struct evaluate *initialize_evaluate(struct expressions_list *el)
{
  struct evaluate *res = malloc(sizeof(struct evaluate));
  if(if_print(el) == PRINT_NODEF)
		res->nb_print = 0;
	else
		res->nb_print = 1;
  res->eval = 0;
  res->step = 0;
  res->current_op = 2;
  return res;
}

/**
** \brief Check if the result of the evaluation is true and then print the
** directory as many times as needed
** \param path  The directory/file
** \param readfile   The struct dirent corresponding to the file
** \param ic   the struct info_command containing details
** about the command line
** \param e   The struct evaluate containing details about the evaluation
** \param last  A string containing the result of the evaluation
*/
void print_result(char *path, struct dirent *readfile, struct info_command *ic,
  struct evaluate *e, char *last)
{
  int evaluation_result = 0;
  if(strcmp(last, "1") != 0 && strcmp(last, "0") != 0)
    evaluation_result = evaluate(path, readfile, last, ic, e);
  else
  {
    if(strcmp(last, "1") == 0)
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
** \brief Process expressions from a given stack
** \param res   The expressions_list that will hold th result
** \param path   The path name
** \param readfile   The struct dirent corresponding to the file
** \param ic   the struct info_command containing details
** about the command line
** \param e   The evaluate structure that contains informations
** about the the evaluation
** \return A stop signal depending on the current operator
*/
int process_expressions(struct expressions_list *res, char *path, struct dirent
  *readfile, struct info_command *ic, struct evaluate *e)
{
  int stop = 0;
  int eval = 0;
  struct expressions_list *aux = reverse_stack(res);
  while(stop == 0 && aux->start != NULL &&
    strcmp(aux->start->data, "1") != 0 &&
    strcmp(aux->start->data, "0") != 0)
  {
    char *expr = pop(aux);
    int expr_eval = evaluate(path, readfile, expr, ic, e);
    if(e->step != 0)
      eval = compare_boolean(expr_eval, eval, e->current_op);
    else
      eval = expr_eval;
    e->eval = eval;
    e->step += 1;
    free(expr);
    if(e->current_op == 0 && eval == 1)
      stop = 1;
  }
  free_exprlist(aux);
  char *data = eval_to_str(eval);
  push(res, data);
  free(data);
  return stop;
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
  while(el->start)
  {
    char *token = pop(el);
    if(strcmp(token, "-o") != 0 && strcmp(token, "-a") != 0)
    {
      push(res, token);
      free(token);
      continue;
    }
    if(strcmp(token, "-o") == 0)
      e->current_op = 0;
    else if(strcmp(token, "-a") == 0)
      e->current_op = 1;
    if(e->current_op != 2)
    {
      int stop = process_expressions(res, path, readfile, ic, e);
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
