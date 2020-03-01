/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dupenv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 01:05:04 by cjamal            #+#    #+#             */
/*   Updated: 2020/03/01 12:50:30 by cjamal           ###   ########.fr       */
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
	DECLARE(t_variables, *cr, *edit_var, *next);
	dup = env2;
	env2 = tmp;
	cr = dup ? dup->head : NULL;
	while (cr)
	{
		next = cr->next;
		edit_var = get_var(cr->key);
		if (edit_var && cr->is_modified == ENV_MODIFIED)
		{
			ft_strdel(&edit_var->value);
			edit_var->value = ft_strdup(cr->value);
		}
		else if (cr->is_modified == ENV_ADDED)
			variable_push(ft_strdup(cr->key), ft_strdup(cr->value), 1, ENV_NT);
		edit_var ? edit_var->is_modified = ENV_NT : 0;
		free_node_env(cr);
		cr = next;
	}
	dup ? free(dup) : 0;
	complete_reset(assign);
}
