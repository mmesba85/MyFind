#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include "myfind.h"
#include "mystrlib.h"

int get_opt(char **argv, struct info_command *ic)
{
  int i = 1;
  while(argv[i])
  {
    if(argv[i][0] == '-')
    {
      if(argv[i][1] == 'd')
      {
        ic->opt_d = OPT_D;
      }
      else if(argv[i][1] == 'H')
      {
        ic->opt = OPT_H;
      }
      else if(argv[i][1] == 'P')
      {
        ic->opt = OPT_DEFAULT;
      }
      else if(argv[i][1] == 'L')
      {
        ic->opt = OPT_L;
      }
      else
        break;
    }
    else
      break;
    i++;
  }
  return i;
}

int get_file(int argc, char **argv, int index, struct info_command *ic)
{
  int nb = 0;
  int i = index;
  int j = 0;
  while(index < argc)
  { 
    if(argv[index][0] == '-' || argv[index][0] == '(')
      break;
    nb++;
    index++;
  }
  char **files = malloc(nb * sizeof(char *));
  if(!files)
    return 1;
  ic->nb_files = nb;
  while(j <= nb && i < argc)
  {
    int len = mystrlen(argv[i]);
    files[j] = malloc(len);
    if(!files[j])
      return 1;
    copy_str(argv[i], files[j], len);
    i++;
    j++; 
  }
  ic->files = files;
  return index;
}

int is_precedence(char *op1, char *op2)
{
  if(mystrcmp(op2, "(") == 0 || mystrcmp(op2, ")") == 0)
  {
    return 0;
  }
   else if(mystrcmp(op1, "-o") == 0 && mystrcmp(op2, "-o"))
  {
    return 1;
  }
  else if(mystrcmp(op1, "-o") == 0 && mystrcmp(op2, "-a"))
  {
    return 0;
  }
  else if(mystrcmp(op1, "-a") == 0 && mystrcmp(op2, "-o"))
  {
    return 1;
  }
  else
    return 1;
}

int check_command(int argc, char **argv, int index)
{
  if(mystrcmp(argv[index], "-name") != 0 && mystrcmp(argv[index], "-type") != 0
    && mystrcmp(argv[index], "-print") != 0 && mystrcmp(argv[index], "-exec") != 0)
  {
    warnx("unknown predicate '%s'", argv[index]);
    return ERROR_PRD;
  }
  if(mystrcmp(argv[index], "-name") == 0 || mystrcmp(argv[index], "-type") == 0
    || mystrcmp(argv[index], "-exec") == 0)
  {
    if(index+1 >= argc)
    {
      warnx("missing argument to '%s'", argv[index]);
      return ERROR_ARG;
    }
  }
  return 0;
}

int get_command(int argc, char **argv, int index, struct expressions_list *output)
{
  int res = check_command(argc, argv, index);
  if(res != 0)
    return res;
  if(mystrcmp(argv[index], "-name") == 0 || mystrcmp(argv[index], "-type") == 0)
  {
    int len = mystrlen(argv[index]);
    char *data = malloc(len + 1);
    if(!data)
      return 1;
    int s = mystrcat(data, argv[index], 0, len);
    index++;
    len += mystrlen(argv[index]);
    data = realloc(data, len+2);
    if(!data)
      return 1;
    data[s] = ' ';
    s = mystrcat(data, argv[index], s+1, len);
    push(output, data);
    free(data);
  }
  return 0;
}

int get_expr(int argc, char **argv, int index, struct info_command *ic)
{
  struct expressions_list *output = initialize();
  struct expressions_list *opr = initialize();
  int aux = 0;
  while(index < argc)
  {
    
    if(mystrcmp(argv[index], "-o") == 0 ||
      mystrcmp(argv[index], "-a") == 0)
    {
      while(opr->start != NULL && is_precedence(argv[index], opr->start->data) == 1)
      {
        char *op = pop(opr);
        push(output, op);
        free(op);
      }
      push(opr, argv[index]);
      index++;
      aux = 0;
    }
    else if(aux == 1 && argv[index][0] != '(' && argv[index][0] != ')')
    {
      while(opr->start != NULL && is_precedence("-a", opr->start->data) == 1)
      {
        char *op = pop(opr);
        push(output, op);
        free(op);
      }
      push(opr, "-a");  
      aux = 0;
    }
    else if(index < argc && argv[index][0] == '-')
    {
      int res = get_command(argc, argv, index, output);
      if(res != 0)
        return res;
      index += 2;
      aux = 1;
    }
    else if(mystrcmp(argv[index], "(") == 0)
    {
      push(opr, argv[index]);
      index++;
    }
    else if(mystrcmp(argv[index], ")") == 0)
    {
      while(mystrcmp(opr->start->data, "(") != 0)
      {
        char *op = pop(opr);
        push(output, op);
        free(op);
        if(!opr)
          return ERROR_PARS;
      }
      pop(opr);
      index++;
    }
  }
  if(opr->start != NULL && mystrcmp(opr->start->data, "\\(") == 0)  
    return ERROR_PARS;
  while(opr->start)
  {
    char *op = pop(opr);
    push(output, op);
    free(op);
  }
  ic->el = output;
  free_path(opr);
  return 0;
}

int myparser(int argc, char **argv, struct info_command *ic)
{
  int opt = get_opt(argv, ic);
  if(opt >= argc)
    return 0;
  int file = get_file(argc, argv, opt, ic);
  if(file >= argc)
    return 0;
  int res = get_expr(argc, argv, file, ic);
  return res;
}

struct info_command *initialize_ic()
{
  struct info_command *ic = malloc(sizeof(struct info_command));
  if(!ic)
    return NULL;
  ic->opt_d = OPT_DEFAULT;
  ic->opt = OPT_DEFAULT;
  ic->nb_files = 0;
  ic->files = NULL;
  ic->el = NULL;
  return ic;
}

void free_ic(struct info_command *ic)
{
  int i = 0;
  if(ic->files != NULL)
  {
    while(i <= ic->nb_files)
    {
      free(ic->files[i]);
      i++;
    }
    free(ic->files);
  }
  if(ic->el != NULL)
    free_path(ic->el);
  free(ic);
}