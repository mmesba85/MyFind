#ifndef MYSTRLIB_H
#define MYSTRLIB_H

#include <stdio.h>

size_t mystrlen(char *str);
void make_path(char *res, char *path, char *dir_name);
int mystrcmp(char *str1, char *str2);

#endif /* MYSTRLIB_H */