#ifndef MYFIND_H
#define MYFIND_H

#include <dirent.h>
#include "stack.h"

#define OPT_DEFAULT 0
#define OPT_D 1
#define OPT_L 2
#define OPT_P 3
#define OPT_H 4
#define PRINT_NODEF 1
#define PRINT_DEF 0
#define MAX_FILE_NAME 255

/**
** \brief The informtions collected while parsing the command line
*/
struct info_command
{

  int opt_d; /*!< The value corresponding to the -d option */
  int opt;   /*!< The value corresponding to the other options */
  int nb_files; /*!< The number of files given in the command line */
  char **files;  /*!< An array containing all files name given in the
                    command line*/
  struct expressions_list *el;  /*!< A stack containing the expressions */
};

/**
** \brief Informations about the evaluation of the expressions_list
*/
struct evaluate
{
  int nb_print; /*!< Determines how many times the file would be printed */
  int current_op; /*! < Corresponds to the operator read */
  int eval;
  int step;
};

int myfind(char *dir_name, struct info_command *ic);
struct info_command *get_info_command(int argc, char **argv);
int check_el(char *path, struct dirent *readfile, struct info_command *ic);
int evaluate(char *path, struct dirent *readfile, char *exp,
  struct info_command *ic, struct evaluate *e);
void free_ic(struct info_command *ic);
struct info_command *initialize_ic();
int myparser(int argc, char **argv, struct info_command *ic);
int exec_command(char *type, char *expr);
int check_name(char *path, struct dirent *readfile, char *expr,
  struct info_command *ic);
int check_type(char *path, struct dirent *readfile, char *expr);


#endif /* MYFIND_H */
