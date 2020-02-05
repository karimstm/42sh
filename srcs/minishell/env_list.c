/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/04 12:02:02 by cjamal            #+#    #+#             */
/*   Updated: 2020/02/04 21:24:47 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "shell.h"

void	init_env_list()
{
	env2->head = NULL;
	env2->tail = NULL;
	env2->node_count = 0;
}

void    variable_push(char *key, char *value, int export)
{
    t_variables *var;

    var = (t_variables*)xmalloc(sizeof(t_variables));
    var->is_exported = export ? 1 : 0;
    var->key = key;
    var->value = value;
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

void	ft_lstonedel(t_variables **to_del)
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

void	delete_var(char *target)
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

t_variables *get_var(char *target)
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

char        **get_tab_env()
{
    t_variables *cur;
    int         i;
    char        **tab_env;
    
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

void    ft_init_env(char **ev)
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
        variable_push(key, value, 1);
        cpy_env++;
    }
}