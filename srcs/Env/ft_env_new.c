/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_new.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/04 15:55:19 by cjamal            #+#    #+#             */
/*   Updated: 2020/03/01 19:11:17 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	ft_var_isvalid(char *str)
{
	if (str && (!ft_isalpha(*str) && *str != '_'))
		return (0);
	if (!str)
		return (0); 
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		str++;
	}
	return (1);
}

int	edit_add_var(char *key, char *value, int is_exported, int mod)
{
	t_variables	*new_var;

	if (!ft_var_isvalid(key))
	{
		ft_printf_fd(2, "42sh: `export': %s: not a valid identifier\n", key);
		return (1);
	}
	if ((new_var = get_var(key)))
	{
		ft_strdel(&new_var->value);
		new_var->value = ft_strdup(value);
		new_var->is_exported = is_exported;
		new_var->is_modified = mod ? ENV_MODIFIED : ENV_DEFAULT;
	}
	else if (mod)
		variable_push(ft_strdup(key), ft_strdup(value), is_exported, ENV_ADDED);
	else
		variable_push(ft_strdup(key), ft_strdup(value),
				is_exported, ENV_DEFAULT);
	return (0);
}

int	ft_export(char **args)
{
	t_variables	*new_var;
	char		*index;
	int			i;

	i = -1;
	if (!args)
		return (1);
	while (args[++i])
	{
		if (ft_strchr(args[i], '=') && (index = ft_strreplace(args[i], '=', 0)))
			edit_add_var(args[i], index, 1, ENV_MODIFIED);
		else if ((new_var = get_var(args[i])) && !new_var->is_exported)
			new_var->is_exported = 1;
	}
	return (0);
}

int	ft_set_var(char **args, t_simple_command *cmd)
{
	char		*index;
	int			i;
	int			set;

	set = cmd ? 1 : 0;
	i = -1;
	if (args)
	{
		while (args[++i])
			if (ft_strchr(args[i], '=') && (index = ft_strreplace(args[i], '=', 0)))
				edit_add_var(args[i], index, set, ENV_DEFAULT);
	}
	return (0);
}
