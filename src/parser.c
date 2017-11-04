#include <stdlib.h>
#include <stdio.h>
#include "myfind.h"
#include "mystrlib.h"

int get_opt(char **argv, struct info_command *ic)
{
  int i = 1;
  ic->opt_d = OPT_DEFAULT;
  ic->opt = OPT_DEFAULT;
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
      i++;
    }
    else
      break;
  }
  return i;
}

int get_file(char **argv, int index, struct info_command *ic)
{
  while(argv[index])
  {
    if(argv[index][0] == '-')
      break;
    else
    {
      int len = mystrlen(argv[index]);
      char *file = malloc(len);
      copy_str(argv[index], file, len);
      ic->file = file;
    }
    index++;
  }
  return index;
}

int is_precedence(char *op1, char *op2)
{
	if(mystrcmp(op1, "-o") == 0 && mystrcmp(op2, "-o"))
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
	{
		return 1;
	}
}

int get_expr(int argc, char **argv, int index, struct info_command *ic)
{
	struct expressions_list *output = initialize();
	struct expressions_list *opr = initialize();
	while(index < argc)
	{
		if(mystrcmp(argv[index], "-o") == 0 ||
			mystrcmp(argv[index], "-a") == 0)
		{
			if(opr->start != NULL)
			{
				while(is_precedence(argv[index], opr->start->data) == 1)
				{
					char *op = pop(opr);
					push(output, op);
				}
			}
			push(opr, argv[index]);
			index++;
		}
		else if(index < argc && argv[index][0] == '-')
		{
			int len = mystrlen(argv[index]);
			char *data = malloc(len);
			int s = mystrcat(data, argv[index], 0, len);
			index++;
			while(index < argc && argv[index][0] != '-')
			{
				len += mystrlen(argv[index]);
				data = realloc(data, len+1);
				data[s] = ' ';
				s = mystrcat(data, argv[index], s+1, len);
				index++;	
			}
			push(output, data);
		}
		else if(mystrcmp(argv[index], "\\(") == 0)
			push(opr, argv[index]);
		else if(mystrcmp(argv[index], "\\)") == 0)
		{
			while(mystrcmp(opr->start->data, "\\(") != 0)
			{
				char *op = pop(opr);
				push(output, op);
				if(!opr)
					return 1;
			}
			pop(opr);
		}
	}
	if(opr->start != NULL && mystrcmp(opr->start->data, "\\(") == 0)
		return 1;
	while(opr->start)
	{
		char *op = pop(opr);
		push(output, op);
	}
	ic->el = output;
	return 0;
}

struct info_command *get_info_command(int argc, char **argv)
{
  argc = argc;
  struct info_command *ic = malloc(sizeof(struct info_command));
  int opt = get_opt(argv, ic);
  int file = get_file(argv, opt, ic);
  printf("%d\n", file);
  if(file >= argc)
  	return ic;
  get_expr(argc, argv, file, ic);
  return ic;
}
