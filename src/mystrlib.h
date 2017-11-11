#ifndef MYSTRLIB_H
#define MYSTRLIB_H

#include <stdio.h>

size_t mystrlen(char *str);
void make_path(char *res, char *path, char *dir_name);
int mystrcmp(char *str1, char *str2);
void copy_str(char *src, char *dest, size_t len);
int mystrcat(char *res, char *str, int b, int res_len);
char *get_type(char *expr);
int str_contains(char *str, char *cnt);

#endif /* MYSTRLIB_H */
