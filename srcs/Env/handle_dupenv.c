/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dupenv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 01:05:04 by cjamal            #+#    #+#             */
/*   Updated: 2020/02/29 11:43:45 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	free_node_env(t_variables *node)
{
	if (node)
	{
		ft_strdel(&node->key);
		ft_strdel(&node->value);
		free(node);
	}
}

void	ft_del_env(void)
{
	t_variables *cur;
	t_variables *next;

	cur = env2 ? env2->head : 0;
	while (cur)
	{
		next = cur->next;
		free_node_env(cur);
		cur = next;
	}
	env2 ? free(env2) : 0;
}

int		is_assigned(char **assign, char *key)
{
	int i;

	i = 0;
	if (assign && key)
	{
		while (assign[i])
		{
			if (ft_strequ(assign[i], key))
				return (1);
			i++;
		}
	}
	return (0);
}

void	complete_reset(char **assign)
{
	t_variables *cur;

	cur = env2 ? env2->head : 0;
	while (cur)
	{
		if (cur->is_modified == 0 && !is_assigned(assign, cur->key))
			delete_var(cur->key);
		cur = cur->next;
	}
}

void	reset_env(t_variables_list *tmp, char **assign)
{
	t_variables_list *dup;

	if (!tmp)
		return ;
	DECLARE(t_variables, *cur_dup, *edit_var);
	dup = env2;
	env2 = tmp;
	cur_dup = dup ? dup->head : NULL;
	while (cur_dup)
	{
		edit_var = get_var(cur_dup->key);
		if (edit_var && cur_dup->is_modified == ENV_MODIFIED)
		{
			ft_strdel(&edit_var->value);
			edit_var->value = ft_strdup(cur_dup->value);
		}
		else if (cur_dup->is_modified == ENV_ADDED)
			variable_push(ft_strdup(cur_dup->key),
					ft_strdup(cur_dup->value), 1, ENV_NOTDELETED);
		edit_var ? edit_var->is_modified = ENV_NOTDELETED : 0;
		free_node_env(cur_dup);
		cur_dup = cur_dup->next;
	}
	dup ? free(dup) : 0;
	complete_reset(assign);
}
