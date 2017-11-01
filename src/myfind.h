#ifndef MYFIND_H
#define MYFIND_H

struct expressions_list
{
  char *exp;
};


int myfind(char *dir_name, struct expressions_list *el, int is_d, int is_h, int is_l, int is_p);

#endif /* MYFIND_H */
