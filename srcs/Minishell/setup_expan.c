/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_expan.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/21 20:15:41 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/26 15:57:24 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_list_simple_command	*do_globing(char *node, t_token_kind kind)
{
	glob_t					glob_holder;
	t_list_simple_command	*glob_lst;
	int						i;

	i = -1;
	glob_lst = malloc_list_simple_command();
	glob(node, 0, 0, &glob_holder);
	if (glob_holder.gl_pathv)
		while (glob_holder.gl_pathv[++i])
			token_push(glob_lst, ft_strdup(glob_holder.gl_pathv[i]), kind);
	else
		token_push(glob_lst, ft_strdup(node), kind);
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


