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

struct info_command *get_info_commad(int argc, char **argv)
{
  argc = argc;
  struct info_command *ic = malloc(sizeof(struct info_command));
  int opt = get_opt(argv, ic);
  int file = get_file(argv, opt, ic);
  file = file;
  //int expr = get_expr(argc, argv, ic, file);
  return ic;
}
