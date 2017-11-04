#include <stdio.h>
#include "myfind.h"

int main(int argc, char **argv)
{
  struct info_command *ic = get_info_command(argc, argv);
  print_s(ic->el);
  return 0;
}