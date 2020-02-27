/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_expan.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/21 20:15:41 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/27 15:08:38 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "globing.h"

char					*ft_glob_dup(char *string)
{
	t_string	s;

	s.string = NULL;
	new_string(&s);
	while (*string)
	{
		if (*string == '"' || *string == '\'' || *string == '\\')
			push(&s, '\\');
		push(&s, *string++);
	}
	return s.string;
}

t_list_simple_command	*do_globing(char *node, t_token_kind kind)
{
	t_glob					glob_holder;
	t_list_simple_command	*glob_lst;
	int						i;

	i = -1;
	glob_lst = malloc_list_simple_command();
	_glob(ft_strdup(node), 0, 0, &glob_holder);
	if (glob_holder.gl_pathv && *glob_holder.gl_pathv != 0)
		while (glob_holder.gl_pathv[++i])
			token_push(glob_lst, ft_glob_dup(glob_holder.gl_pathv[i]), kind);
	else
		token_push(glob_lst, ft_strdup(node), kind);
	i = -1;
	if (glob_holder.gl_pathv)
	{	
		while (glob_holder.gl_pathv[++i])
			ft_strdel(&glob_holder.gl_pathv[i]);
		free(glob_holder.gl_pathv);
	}
	return (glob_lst);
}

void	init_globing(t_list_simple_command *list)
{
	t_simple_command		*current;
	t_simple_command		*next;
	t_simple_command		*prev;
	t_list_simple_command	*res;
	
	current = list && list->head ? list->head : NULL;
	prev = NULL;
	while (current)
	{
		next = current->next;
		ft_putendl(current->name);
		if ((res = do_globing(current->name, current->kind))->node_count)
		{
			if (prev)
				prev->next = res->head;
			else
				list->head = res->head;
			prev = res->tail;
			list->node_count += (res->node_count - 1);
		}
		free(res);
		ft_strdel(&current->name);
		free(current);
		current = next;
	}
	if (prev)
		list->tail = prev;
}

int		setup_expan(t_list_simple_command *list)
{
	setup_sub_proce(list);
	expand_args(list);
	clean_list(list);
	init_expansion(list);
	init_globing(list);
	return (1);
}
