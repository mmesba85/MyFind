/**
** \file stack.c
** \brief basic functions to manipulate a stack structure
** \author Maroua Mesbahi
** \version 1.0
** \date 12/11/2017
** the file contains operations such as pop,
** push or delete
*/

#include <stdlib.h>
#include <stdio.h>
#include "mystrlib.h"
#include "myfind.h"





/**
** \brief Initialize a struct expressions_list
** \return new expressions_list structure with a start
** node initialized to NULL
*/
struct expressions_list *initialize_stack()
{
  struct expressions_list *new = malloc(sizeof(struct expressions_list));
  if (!new)
    return NULL;
  new->start = NULL;
  return new;
}

/**
** \brief push the element data to the stack
** expressions_list
** \param el 	expressions_list structure
** \param data 	string containing the expression
*/
void push(struct expressions_list *el, char *data)
{
  struct stack *new = malloc(sizeof(struct stack));
  if (!new || !el)
    return;
  size_t len = mystrlen(data);
  char *aux = calloc(len+1, sizeof(char));
  copy_str(data, aux, len+1);
  new->data = aux;
  new->next = el->start;
  el->start = new;
}

/**
** \brief pop the last element added to
** the expressions_list
** \param el 	expressions_list structure
** \return a string containing the last element added to
** the expressions_list
*/
char *pop(struct expressions_list *el)
{
  if (!el)
    return NULL;
  struct stack *s = el->start;
  if(s)
  {
    size_t len = mystrlen(s->data);
    char *res = calloc(len+1, sizeof(char));
    if(!res)
    	return NULL;
    copy_str(s->data, res, len+1);
    free(s->data);
    el->start = s->next;
    free(s);
    return res;
  }
  return NULL;
}

void print_s(struct expressions_list *el)
{
  if(!el)
    return;
  struct stack *tmp = el->start;
  while(tmp)
  {
    printf("%s\n", tmp->data);
    tmp = tmp->next;
  }
}

/**
** \brief free the allocated memory used by
** the expressions_list
** \param el 	expressions_list structure that needs
** to be freed
*/
void free_exprlist(struct expressions_list *el)
{
  while(el->start)
  {
   	char *aux = pop(el);
   	free(aux);
  }
  free(el);
}

/**
** \brief create a reversed copy of the expressions_list
** \details it is useful when evaluating the list
** for each file
** \param el 	the expressions_list that will be reversed
** \return a reversed copy of the expressions_list given
** as parameter
*/
struct expressions_list *reverse_stack(struct expressions_list *el)
{
	struct expressions_list *res = initialize_stack();
	struct expressions_list *tmp = initialize_stack();
	while(el->start)
	{
		char *data = pop(el);
		push(tmp, data);
		push(res, data);
		free(data);
	}
	while(tmp->start)
	{
		char *data = pop(tmp);
		push(el, data);
		free(data);
	}
	free_exprlist(tmp);
	return res;
}


/**
** \brief Check if the stack contains a particular string
** \param list  The expressions_list that will be checked
** \param str   The string that we are looking for
** in the list
** \details Useful when checking if a command line
** contains '-print'
** to decide how the result would be printed
*/
int stack_contains(struct expressions_list *list, char *str)
{
  struct expressions_list *el = reverse_stack(list);
  while(el->start)
  {
    char *data = pop(el);
    if(mystrcmp(data, "-print") == 0 &&
    mystrcmp(str, "-print") == 0)
    {
      free_exprlist(el);
      free(data);
      return 0;
    }
    else
    {
      if(mystrcmp(str, "-name") == 0 &&
      str_contains(data, "-name") == 0)
      {
          free_exprlist(el);
          free(data);
          return 0;
      }
      if(mystrcmp(str, "-type") == 0 &&
      str_contains(data, "-type") == 0)
      {
        free_exprlist(el);
        free(data);
        return 0;
      }
    }
    free(data);
  }
  free_exprlist(el);
  return 1;
}
