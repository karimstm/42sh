#include "shell.h"


int		get_size(t_stack *sp)
{
	return (sp->size);
}

int is_overflow(t_stack *sp){
	return sp->top == sp->size - 1;

}
int is_underflow(t_stack *sp){
	return sp->top == -1;
}

void init_stack(t_stack *sp, int size){
	sp->top = -1;
	sp->lists = (t_node **)xmalloc(sizeof(t_node *) * size);
	sp->size = size;
}


void	push_to_stack(t_stack *sp, t_node *list)
{
	t_node 	**tmp;
	int		i;

	i = 0;
	if (is_overflow(sp))
	{
		tmp = (t_node **)xmalloc(sizeof(t_node *) * sp->size * 2);
		while (i < sp->top)
		{
			tmp[i] = sp->lists[i];
			i++;
		}
		free(sp->lists);
		sp->lists = tmp;
		sp->size *= 2;
	}
	sp->top++;
    sp->lists[sp->top] = list;
}

void		reverse_stack(t_stack *sp)
{
	t_node **tmp;
	int		i;
	int		j;

	i = sp->top;
	j = 0;
	tmp = (t_node **)xmalloc(sizeof(t_node *) * sp->size);
	while (i >= 0)
	{
		tmp[j++] = sp->lists[i];
		i--;
	}
	free(sp->lists);
	sp->lists = tmp;
}

t_node		*pop_stack(t_stack *sp)
{
	t_node		*value;

	if (is_underflow(sp)){
		ft_printf_fd(2, "Stack Underflow\n");
		return (NULL);
	}
	value = sp->lists[sp->top];
	sp->top--;
	return (value);
}


void deallocate(t_stack *sp){
	if (sp->lists != NULL)
		free(sp->lists);
	sp->top = -1;
	sp->size = 0;
}
