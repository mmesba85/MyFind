#ifndef MYFIND_H
#define MYFIND_H

#define OPT_DEFAULT 0
#define OPT_D 1
#define OPT_L 2
#define OPT_P 3
#define OPT_H 4

#include <dirent.h>

#define MAX_FILE_NAME 255
#define ERROR_ARG 1
#define ERROR_PARS 2
#define ERROR_PRD 3
#define NO_DIR 4

/**
** \brief The informtions collected while parsing the command line
*/
struct info_command
{
  int opt_d; /*!< The value corresponding to the -d option */
  int opt;   /*!< The value corresponding to the other options */ 
  int nb_files; /*!< The number of files given in the command line */
  char **files;  /*!< An array containing all files name given in the command line*/
  struct expressions_list *el;  /*!< A stack containing the expressions */
};

/**
** \brief The expressions_list structure
** \details It is supposed to hold all the expressions given
** in the command line
*/
struct expressions_list
{
  struct stack *start; /*!< The first node of the structure */
};

/**
** \brief The stack structure
** \details It is the first node of the expressions_list structure
*/
struct stack
{
  char *data;	/*!< The first element of the stack */
  struct stack *next;	/*!< A pointer to the next element */
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
struct info_command *initialize_ic();
int myparser(int argc, char **argv, struct info_command *ic);

#endif /* MYFIND_H */
