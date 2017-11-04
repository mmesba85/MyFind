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
struct info_command *get_info_command(int argc, char **argv);
struct expressions_list *initialize();
void push(struct expressions_list *el, char *data);
char *pop(struct expressions_list *el);
void print_s(struct expressions_list *el);

#endif /* MYFIND_H */
