/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_new.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/04 15:55:19 by cjamal            #+#    #+#             */
/*   Updated: 2020/02/10 19:37:54 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "shell.h"

/*
export name=value => add name to ENV && set
export name => add name to ENV if name is in set
name=value => add name to set || modify name if name is already in set or ENV
unset name => retrieve name from set and ENV
HOME=/tmp cd => exec cd with a dup ENV (setenv)
*/

int		ft_var_isvalid(char *str)
{
    if (str && (!ft_isalpha(*str) && *str != '_'))
        return (0);
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		str++;
	}
	return (1);
}

int    edit_add_var(char *key, char *value, int is_exported)
{
    t_variables *new_var;

    if (!ft_var_isvalid(key))
    {
        ft_printf_fd(2, "42sh: `export': %s: not a valid identifier\n", key);
        return (1);
    }
    if ((new_var = get_var(key)))
    {
        ft_strdel(&new_var->value);
        new_var->value = ft_strdup(value);
    }
    else
        variable_push(ft_strdup(key), ft_strdup(value), is_exported);
    return (0);
}

int     ft_export(char **args)
{
    t_variables *new_var;
    char        *index;
    int i;

    i = -1;
    while (args[++i])
    {
        if (ft_strchr(args[i], '=') && (index = ft_strreplace(args[i], '=', 0)))
            edit_add_var(args[i], index , 1);
        else if ((new_var = get_var(args[i])) && !new_var->is_exported)
            new_var->is_exported = 1; 
    }
    return (0);
}

int     ft_unset(char **args)
{
    int i;

    i = -1;
    if (!args)
        return (1);
    while (args[++i])
    {
        if (!ft_var_isvalid(args[i]))
            ft_printf_fd(2, "42sh: export: `%s': not a valid identifier\n", args[i]);
        else
            delete_var(args[i]);
    }
    return (0);
}

int     ft_set(char **args)
{
    t_variables *cur;
    char *index;
    int i;

    if (args && *args)
    {
        i = -1;
        while (args[++i])
            if (ft_strchr(args[i], '=') && (index = ft_strreplace(args[i], '=', 0)))
                    edit_add_var(args[i], index , 0);
    }
    else
    { 
        cur = env2->head;
        while (cur)
        {
            ft_printf("%s=%s\n", cur->key, cur->value);
            cur = cur->next;
        }
    }
    return (0);
}

int     ft_env(char **args)
{
    t_variables *cur;

    (void)args;
    cur = env2->head;
    while (cur)
    {
        if (cur->is_exported)
            ft_printf("%s=%s\n", cur->key, cur->value);
        cur = cur->next;
    }
    return (0);
}