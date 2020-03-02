/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/04 12:02:02 by cjamal            #+#    #+#             */
/*   Updated: 2020/03/02 16:33:23 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_variables	*get_var(char *target)
{
	t_variables	*current;

	current = g_env ? g_env->head : 0;
	while (current)
	{
		if (ft_strequ(current->key, target))
			return (current);
		current = current->next;
	}
	return (NULL);
}

void		variable_push(char *key, char *value, int export, int is_modified)
{
	t_variables	*var;

	var = (t_variables*)xmalloc(sizeof(t_variables));
	var->is_exported = export ? 1 : 0;
	var->key = key;
	var->value = value;
	var->is_modified = is_modified;
	var->next = NULL;
	if (g_env->node_count == 0)
		g_env->head = var;
	else
		g_env->tail->next = var;
	g_env->tail = var;
	g_env->node_count++;
}

t_variables	**find_var(char *target, t_variables **prev)
{
	t_variables	**current;

	current = g_env ? &g_env->head : 0;
	*prev = NULL;
	while (current && *current)
	{
		if (ft_strequ((*current)->key, target))
			return (current);
		*prev = *current;
		current = &(*current)->next;
	}
	return (NULL);
}

void		ft_lstonedel(t_variables **to_del)
{
	t_variables	*tmp;

	if (to_del && *to_del)
	{
		tmp = *to_del;
		*to_del = tmp->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

void		delete_var(char *target)
{
	t_variables	**to_del;
	t_variables	*prev;

	if (!g_env || g_env->node_count == 0)
		return ;
	if ((to_del = find_var(target, &prev)))
	{
		if (g_env->node_count == 1)
		{
			g_env->head = NULL;
			g_env->tail = NULL;
		}
		if (*to_del == g_env->head && g_env->node_count > 1)
			g_env->head = (*to_del)->next;
		else if (*to_del == g_env->tail && g_env->node_count > 1)
			g_env->tail = prev;
		ft_lstonedel(to_del);
		g_env->node_count--;
	}
}
