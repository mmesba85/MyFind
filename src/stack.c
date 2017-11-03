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
  new->data = data;
  new->next = el->start;
  el->start = new;
}

char *pull(struct expressions_list *el)
{
  if (!el)
    return NULL;
  struct stack *s = el->start;
  if(s)
  {
    size_t len = mystrlen(s->data);
    char *res = malloc(len);
    copy_str(s->data, res, len);
    el->start = s->next;
    free(s);
    return res;
  }
  return NULL;
}

void print_path(struct expressions_list *el)
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
    pull(el);
  free(el);
}

