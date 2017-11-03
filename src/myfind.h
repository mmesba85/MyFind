#ifndef MYFIND_H
#define MYFIND_H

#define OPT_DEFAULT 0
#define OPT_D 1
#define OPT_L 2
#define OPT_P 3
#define OPT_H 4

struct info_command
{
  int opt_d;
  int opt;
  char *file;
  struct expressions_list *el;
};


struct expressions_list
{
  struct stack *start;
};

struct stack
{
  char *data;
  struct stack *next;
};


int myfind(char *dir_name, struct expressions_list *el, int is_d, int is_h, int is_l, int is_p);

#endif /* MYFIND_H */
