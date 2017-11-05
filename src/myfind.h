#ifndef MYFIND_H
#define MYFIND_H

#define OPT_DEFAULT 0
#define OPT_D 1
#define OPT_L 2
#define OPT_P 3
#define OPT_H 4

#include <dirent.h>

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


int myfind(char *dir_name, struct info_command *ic);
struct info_command *get_info_command(int argc, char **argv);
struct expressions_list *initialize();
void push(struct expressions_list *el, char *data);
char *pop(struct expressions_list *el);
void print_s(struct expressions_list *el);
int check_el(char *path, struct dirent *readfile, struct info_command *ic);
int evaluate(char *path, struct dirent *readfile, char *exp);
struct expressions_list *copy_stack(struct expressions_list *el);
void free_path(struct expressions_list *el);
void free_ic(struct info_command *ic);
#endif /* MYFIND_H */
