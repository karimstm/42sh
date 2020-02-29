/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_env2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 01:12:17 by cjamal            #+#    #+#             */
/*   Updated: 2020/02/29 01:15:33 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "shell.h"

t_variables         *dup_node(t_variables *node_to_dup)
{
	t_variables     *dup_node;

	if (node_to_dup)
	{
		dup_node = (t_variables*)xmalloc(sizeof(t_variables));
		dup_node->is_exported = node_to_dup->is_exported;
		dup_node->key = ft_strdup(node_to_dup->key);
		dup_node->value = ft_strdup(node_to_dup->value);
		dup_node->is_modified = ENV_DEFAULT;
		node_to_dup->is_modified = ENV_DEFAULT;
		return (dup_node);
	}
	return (NULL);
}

t_variables_list    *dup_env(void)
{
	t_variables_list *dup;
	t_variables     *cur;
	t_variables     *cur_dup;

	dup = (t_variables_list *)xmalloc(sizeof(t_variables_list));;
	dup->tail = NULL;
	dup->node_count = env2 ? env2->node_count : 0;
	cur = env2 ? env2->head : 0;
	dup->head = dup_node(cur);
	cur_dup = dup ? dup->head : 0;
	while (cur)
	{
		cur_dup->next = dup_node(cur->next);
		if (!cur->next)
			dup->tail = cur_dup;
		cur = cur->next;
		cur_dup = cur_dup->next;
	}
	return (dup);
}

char			**get_tab_env()
{
	t_variables		*cur;
	int				i;
	char			**tab_env;

	i = 0;
	tab_env = NULL;
	if (env2->node_count)
	{
		tab_env = (char **)xmalloc(sizeof(char*) * (env2->node_count + 1));
		cur = env2->head;
		while (cur)
		{
			tab_env[i++] = ft_strjoin_pre(cur->key, "=", cur->value);
			cur = cur->next;
		}
		tab_env[i] = NULL;
	}
	return (tab_env);
}

void	init_env_list()
{
	env2->head = NULL;
	env2->tail = NULL;
	env2->node_count = 0;
}

void		ft_init_env(char **ev)
{
	char **cpy_env;
	char *index;
	char *key;
	char *value;

	env2 = (t_variables_list*)xmalloc(sizeof(t_variables_list));
	init_env_list();
	cpy_env = ev;
	while (*cpy_env)
	{
		index = ft_strchr(*cpy_env, '=');
		key = ft_strsub(*cpy_env, 0, index - *cpy_env);
		value = ft_strdup(index + 1);
		variable_push(key, value, 1, 0);
		cpy_env++;
	}
}