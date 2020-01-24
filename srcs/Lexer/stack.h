#ifndef STACK_H
# define STACK_H
# include "ast.h"

typedef struct		s_stack
{
	t_node			**lists;
	int				size;
	int				top;
}					t_stack;

void				init_stack(t_stack *sp, int size);
void				push_to_stack(t_stack *sp, t_node *list);
void				reverse_stack(t_stack *sp);
t_node				*pop_stack(t_stack *sp);
void				deallocate(t_stack *sp);
int					is_underflow(t_stack *sp);
int					is_overflow(t_stack *sp);
int					get_size(t_stack *sp);

#endif
