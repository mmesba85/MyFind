#ifndef STACK_H
#define STACK_H

/*
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
  char *data;  /*!< The first element of the stack */
  struct stack *next;  /*!< A pointer to the next element */
};

struct expressions_list *initialize_stack();
void push(struct expressions_list *el, char *data);
char *pop(struct expressions_list *el);
void print_s(struct expressions_list *el);
struct expressions_list *reverse_stack(struct expressions_list *el);
void free_exprlist(struct expressions_list *el);
int stack_contains(struct expressions_list *el, char *str);

#endif /* STACK_ */
