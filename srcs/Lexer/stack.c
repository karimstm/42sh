/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 16:12:26 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 16:13:08 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void		init_stack(t_stack *sp, int size)
{
	sp->top = -1;
	sp->lists = (t_node **)xmalloc(sizeof(t_node *) * size);
	sp->size = size;
}

void		push_to_stack(t_stack *sp, t_node *list)
{
	t_node	**tmp;
	int		i;

	if (list == NULL)
		return ;
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
	t_node	**tmp;
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

	if (is_underflow(sp))
		return (NULL);
	value = sp->lists[sp->top];
	sp->top--;
	return (value);
}

void		deallocate(t_stack *sp)
{
	if (sp->lists != NULL)
		free(sp->lists);
	sp->top = -1;
	sp->size = 0;
}
