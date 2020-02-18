/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/04 12:02:02 by cjamal            #+#    #+#             */
/*   Updated: 2020/02/18 18:17:10 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "shell.h"

void	init_env_list()
{
	env2->head = NULL;
	env2->tail = NULL;
	env2->node_count = 0;
}

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

void		variable_push(char *key, char *value, int export, int is_modified)
{
	t_variables *var;

	var = (t_variables*)xmalloc(sizeof(t_variables));
	var->is_exported = export ? 1 : 0;
	var->key = key;
	var->value = value;
	var->is_modified = is_modified;
	var->next = NULL;
	if (env2->node_count == 0)
		env2->head = var;
	else
		env2->tail->next = var;
	env2->tail = var;
	env2->node_count++;
}

t_variables	**find_var(char *target, t_variables **prev)
{
	t_variables **current;

	current = &env2->head;
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
	t_variables **to_del;
	t_variables *prev;

	if (env2->node_count == 0)
		return ;
	if ((to_del = find_var(target, &prev)))
	{
		if (env2->node_count == 1)
		{
			env2->head = NULL;
			env2->tail = NULL;
		}
		if (*to_del == env2->head && env2->node_count > 1)
			env2->head = (*to_del)->next;
		else if (*to_del == env2->tail && env2->node_count > 1)
			env2->tail = prev;
		ft_lstonedel(to_del);
		env2->node_count--;
	}
}

t_variables		*get_var(char *target)
{
	t_variables *current;

	current = env2->head;
	while (current)
	{
		if (ft_strequ(current->key, target))
			return (current);
		current = current->next;
	}
	return (NULL);
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

void		free_node_env(t_variables *node)
{
	if (node)
	{
		ft_strdel(&node->key);
		ft_strdel(&node->value);
		free(node);
	}
}

int			is_assigned(char **assign, char *key)
{
	int i;

	i = 0;
	while (assign[i])
	{
		if (ft_strequ(assign[i], key))
			return (1);
		i++;
	}
	return (0);
}

void		complete_reset(char **assign)
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

void		reset_env(t_variables_list *tmp, char **assign)
{
    t_variables_list    *dup;

    if (tmp)
    {
        DECLARE(t_variables , *cur_dup, *edit_var);
        dup = env2;
        env2 = tmp;
        cur_dup = dup ? dup->head : NULL;
        while (cur_dup)
        {
            if ((edit_var = get_var(cur_dup->key)) && cur_dup->is_modified == ENV_MODIFIED)
            {
                ft_strdel(&edit_var->value);
                edit_var->value = ft_strdup(cur_dup->value);
            }
            else if (cur_dup->is_modified == ENV_ADDED)
                variable_push(ft_strdup(cur_dup->key), ft_strdup(cur_dup->value), 1, ENV_NOTDELETED);
            edit_var ? edit_var->is_modified = ENV_NOTDELETED : 0;
            free_node_env(cur_dup);
            cur_dup = cur_dup->next;
        }
		dup ? free(dup) : 0;
        complete_reset(assign);
    }
}
