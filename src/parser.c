/**
** \file parser.c
** \brief Parse the command line
** \author Maroua Mesbahi
** \version 1.0
** \date 12/11/2017
**  Using the command line, and while parsing it, this file shall fill
**  the fields of the command_info structure
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include "myfind.h"
#include "mystrlib.h"

/**
** \brief Get the option given by the user
** \param argv  Array of strings containing the command line
** \param ic  The info_command that needs to be filled
** \return The new offset
*/
int get_opt(char **argv, struct info_command *ic)
{
  int i = 1;
  while(argv[i])
  {
    if(argv[i][0] == '-')
    {
      if(argv[i][1] == 'd')
      {
        ic->opt_d = OPT_D;
      }
      else if(argv[i][1] == 'H')
      {
        ic->opt = OPT_H;
      }
      else if(argv[i][1] == 'P')
      {
        ic->opt = OPT_DEFAULT;
      }
      else if(argv[i][1] == 'L')
      {
        ic->opt = OPT_L;
      }
      else
        break;
    }
    else
      break;
    i++;
  }
  return i;
}

/**
** \brief Calculate number of files given to the command
** \param argv  Array of strings containing the command line
** \param index   the offset
** \return The number of files
*/
int get_nb_files(int argc, char **argv, int index)
{
  int nb = 0;
  while(index < argc)
  {
    if(argv[index][0] == '-' || argv[index][0] == '(')
      break;
    nb++;
    index++;
  }
  return nb;
}

/**
** \brief Collect the files name
** \param argv  Array of strings containing the command line
** \param index   the offset
** \param ic  The info_command that needs to be filled
** \return The new offset
*/
int get_file(int argc, char **argv, int index, struct info_command *ic)
{
  int nb = get_nb_files(argc, argv, index);
  int j = 0;
  char **files = NULL;
  if(nb > 0)
  {
    ic->nb_files = nb;
    files = malloc(nb * sizeof(char *));
    if(!files)
      return 1;
    while(j < nb && index < argc)
    {
      int len = mystrlen(argv[index]);
      files[j] = calloc(len+1, sizeof(char));
      if(!files[j])
        return 1;
      copy_str(argv[index], files[j], len+1);
      index++;
      j++;
    }
  }
  else
  {
    ic->nb_files = 1;
    files = malloc(sizeof(char *));
    files[0] = malloc(2);
    files[0][0] = '.';
    files[0][1] = '\0';
  }
  ic->files = files;
  return index;
}

/**
** \brief Check the precedence between two operands
** \param op1   The first operand
** \param op2   The second operand
** \return 0 if the first operand has lower precedence, 1 otherwise
** \details This method is needed while implementing the Shunting Yard Alorithm
*/
int is_precedence(char *op1, char *op2)
{
  if(mystrcmp(op2, "(") == 0 || mystrcmp(op2, ")") == 0)
  {
    return 0;
  }
   else if(mystrcmp(op1, "-o") == 0 && mystrcmp(op2, "-o"))
  {
    return 1;
  }
  else if(mystrcmp(op1, "-o") == 0 && mystrcmp(op2, "-a"))
  {
    return 0;
  }
  else if(mystrcmp(op1, "-a") == 0 && mystrcmp(op2, "-o"))
  {
    return 1;
  }
  else
    return 1;
}

/**
** \brief Check if the given argument to type test is right
** \param c   The character that follows the -type
** \return 0 if it is right, 1 otherwise
*/
int check_type_arg(char c)
{
  if( c == 'f' || c == 'd' || c == 'b' || c == 'c' || c == 'l'
     || c == 'p' || c == 's')
    return 0;
  return 1;
}

/**
** \brief Check if the command is right
** \param argv  Array of strings containing the command line
** \param argc  Size of the array
** \param ic  The info_command that needs to be filled
** \return
** \details Check if the command has the right name
** and the right number of arguments
*/
int check_command(int argc, char **argv, int index)
{
  if(mystrcmp(argv[index], "-name") != 0 && mystrcmp(argv[index], "-type") != 0
    && mystrcmp(argv[index], "-print") != 0 && mystrcmp(argv[index], "-exec") != 0)
  {
    warnx("unknown predicate '%s'", argv[index]);
    return 1;
  }
  if(mystrcmp(argv[index], "-name") == 0 || mystrcmp(argv[index], "-type") == 0
    || mystrcmp(argv[index], "-exec") == 0)
  {
    if(index+1 >= argc)
    {
      warnx("missing argument to '%s'", argv[index]);
      return 1;
    }
    else
    {
      if(mystrcmp(argv[index], "-type") == 0)
      {
        int check = check_type_arg(argv[index+1][0]);
        if(check != 0)
        {
          warnx("Unknown argument to -type: %c", argv[index+1][0]);
          return 1;
        }
      }
    }
  }
  return 0;
}

/**
** \brief Push the test coommand into the stack
** \param argv  Array of strings containing the command line
** \param index  The offset
** \param output  The output stack
*/
int push_test(char **argv, int index, struct expressions_list *output)
{
  int len = mystrlen(argv[index]);
  char *data = malloc(len + 1);
  if(!data)
    return 1;
  int s = mystrcat(data, argv[index], 0, len);
  index++;
  len += mystrlen(argv[index]);
  data = realloc(data, len+2);
  if(!data)
    return 1;
  data[s] = ' ';
  s = mystrcat(data, argv[index], s+1, len);
  push(output, data);
  free(data);
  return 0;
}

/**
** \brief Collect the action/test name and its argument (if exists)
** \param argc  Size of the array
** \param argv  Array of strings containing the command line
** \param index  The offset
** \param output  The output stack
** \return 0 if success, 1 otherwise
*/
int get_command(int argc, char **argv, int index, struct expressions_list *output)
{
  int res = check_command(argc, argv, index);
  if(res != 0)
    return res;
  if(mystrcmp(argv[index], "-name") == 0 || mystrcmp(argv[index], "-type") == 0)
    return push_test(argv, index, output);
  else if(mystrcmp(argv[index], "-print") == 0)
  {
    push(output, "-print");
    return 2;
  }
  return 0;
}
/**
** \brief Fill the expressions_list field of the info_command structure
** \param argc  Size of the array
** \param argv  Array of strings containing the command line
** \param index  The offset
** \param ic  The info_commmand structure
** \return 0 if success, 1 otherwise
** \details This method use the Shunring Yard Algorithm
*/
int get_expr(int argc, char **argv, int index, struct info_command *ic)
{
  struct expressions_list *output = initialize_stack();
  struct expressions_list *opr = initialize_stack();
  int aux = 0;
  while(index < argc)
  {

    if(mystrcmp(argv[index], "-o") == 0 ||
      mystrcmp(argv[index], "-a") == 0)
    {
      while(opr->start != NULL && is_precedence(argv[index], opr->start->data) == 1)
      {
        char *op = pop(opr);
        push(output, op);
        free(op);
      }
      push(opr, argv[index]);
      index++;
      aux = 0;
    }
    else if(aux == 1 && argv[index][0] != '(' && argv[index][0] != ')')
    {
      while(opr->start != NULL && is_precedence("-a", opr->start->data) == 1)
      {
        char *op = pop(opr);
        push(output, op);
        free(op);
      }
      push(opr, "-a");
      aux = 0;
    }
    else if(index < argc && argv[index][0] == '-')
    {
      int res = get_command(argc, argv, index, output);
      if(res == 1)
      {
        free_exprlist(opr);
        free_exprlist(output);
        return res;
      }
      if(res == 0)
        index += 2;
      else
        index += 1;
      aux = 1;
    }
    else if(mystrcmp(argv[index], "(") == 0)
    {
      push(opr, argv[index]);
      index++;
    }
    else if(mystrcmp(argv[index], ")") == 0)
    {
      while(mystrcmp(opr->start->data, "(") != 0)
      {
        char *op = pop(opr);
        push(output, op);
        free(op);
        if(!opr)
          return 1;
      }
      pop(opr);
      index++;
    }
    else
    {
      free_exprlist(output);
      free_exprlist(opr);
      warnx("paths must precede expression: %s", argv[index]);
      return 1;
    }
  }
  if(opr->start != NULL && mystrcmp(opr->start->data, "\\(") == 0)
    return 1;
  while(opr->start)
  {
    char *op = pop(opr);
    push(output, op);
    free(op);
  }
  ic->el = output;
  free_exprlist(opr);
  return 0;
}

/**
** \brief The main parsing function
** \param argc  The Arguments array size
** \param argv  The arguments array
** \param ic  The info_command structure that needs to be filled
** \return 0 if success, 1 otherwise
*/
int myparser(int argc, char **argv, struct info_command *ic)
{
  int opt = get_opt(argv, ic);
  if(opt >= argc)
    return 0;

  int file = get_file(argc, argv, opt, ic);
  if(file >= argc)
    return 0;
  int res = get_expr(argc, argv, file, ic);
  return res;
}

/**
** \brief Initialize a new info_command structure
** \return The new initialized info_command
** \details Options are set to default, the number of files to 0
** and the expressions_list to null
*/
struct info_command *initialize_ic()
{
  struct info_command *ic = malloc(sizeof(struct info_command));
  if(!ic)
    return NULL;
  ic->opt_d = OPT_DEFAULT;
  ic->opt = OPT_DEFAULT;
  ic->nb_files = 0;
  ic->files = NULL;
  ic->el = NULL;
  return ic;
}

/**
** \brief Free the info_command structure
** \param ic   The info_command structure that needs to be freed
*/
void free_ic(struct info_command *ic)
{
  int i = 0;
  if(ic->files != NULL)
  {
    while(i < ic->nb_files)
    {
      free(ic->files[i]);
      i++;
    }
    free(ic->files);
  }
  if(ic->el != NULL)
    free_exprlist(ic->el);
  free(ic);
}
