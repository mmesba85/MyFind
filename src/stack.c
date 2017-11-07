#include <stdlib.h>
#include <stdio.h>
#include "mystrlib.h"
#include "myfind.h"

struct expressions_list *initialize()
{
  struct expressions_list *new = malloc(sizeof(struct expressions_list));
  if (!new)
    return NULL;
  new->start = NULL;
  return new;
}

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

void free_path(struct expressions_list *el)
{
  while(el->start)
  {
   	char *aux = pop(el);
   	free(aux);
  }
  free(el);
}

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


