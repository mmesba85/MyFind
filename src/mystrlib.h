#ifndef MYSTRLIB_H
#define MYSTRLIB_H

#include <stdio.h>

void make_path(char *res, char *path, char *dir_name);
int mystrcat(char *res, char *str, int b, int res_len);
char *get_type(char *expr);
int str_contains(char *str, char *cnt);

#endif /* MYSTRLIB_H */
