#include <stdio.h>
#include "myfind.h"

int main(int argc, char **argv)
{
  struct info_command *ic = get_info_command(argc, argv);
  if(ic->file == NULL)
  	myfind(".", ic);
  else
  	myfind(ic->file, ic);
  free_ic(ic);
  return 0;
}