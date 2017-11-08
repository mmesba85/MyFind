/**
** \file stack.c
** \brief basic functions to manipulate a stack structure
** \author Maroua Mesbahi
** \version 1.0
** \date 12/11/2017
** the file contains operations such as pop, push or delete
*/

#include <stdlib.h>
#include <stdio.h>
#include "mystrlib.h"
#include "myfind.h"





/**
** \brief Initialize a struct expressions_list
** \return new expressions_list structure with a start node initialized to NULL
*/
struct expressions_list *initialize()
{
  struct expressions_list *new = malloc(sizeof(struct expressions_list));
  if (!new)
    return NULL;
  new->start = NULL;
  return new;
}

/**
** \brief push the element data to the stack expressions_list
** \param el 	expressions_list structure
** \param data 	string containing the expression
*/
void push(struct expressions_list *el, char *data)
{
  struct stack *new = malloc(sizeof(struct stack));
  if (!new || !el)
    return;
  size_t len = mystrlen(data);
  char *aux = malloc(len);
  copy_str(data, aux, len);
  new->data = aux;
  new->next = el->start;
  el->start = new;
}

/**
** \brief pop the last element added to the expressions_list
** \param el 	expressions_list structure
** \return a string containing the last element added to the expressions_list
*/
char *pop(struct expressions_list *el)
{
  if (!el)
    return NULL;
  struct stack *s = el->start;
  if(s)
  {
    size_t len = mystrlen(s->data);
    char *res = malloc(len);
    if(!res)
    	return NULL;
    copy_str(s->data, res, len);
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
** \brief free the allocated memory used by the expressions_list
** \param el 	expressions_list structure that needs to be freed
*/
void free_path(struct expressions_list *el)
{
  while(el->start)
  {
   	char *aux = pop(el);
   	free(aux);
  }
  free(el);
}

/**
** \brief create a copy of an expressions list
** \details it is useful when exploring the expressions_list for each file
** \param el 	the expressions_list that will be copied
** \return a copy of the expressions_list given as parameter
*/
struct expressions_list *copy_stack(struct expressions_list *el)
{
	struct expressions_list *res = initialize();
	struct expressions_list *tmp = initialize();
	while(el->start)
	{
		char *data = pop(el);
		push(tmp, data);
		free(data);
	}
	while(tmp->start)
	{
		char *data = pop(tmp);
		push(el, data);
		push(res, data);
		free(data);
	}
	free_path(tmp);
	return res;
}


