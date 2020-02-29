/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 01:07:32 by cjamal            #+#    #+#             */
/*   Updated: 2020/02/29 01:23:21 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	ft_unset(char **args)
{
	int i;

	i = -1;
	if (!args)
		return (1);
	while (args[++i])
	{
		if (!ft_var_isvalid(args[i]))
			ft_printf_fd(2, "42sh: export: `%s': not a valid identifier\n",
					args[i]);
		else
			delete_var(args[i]);
	}
	return (0);
}

int	ft_set(char **args)
{
	t_variables *cur;

	(void)args;
	cur = env2->head;
	while (cur)
	{
		ft_printf("%s=%s\n", cur->key, cur->value);
		cur = cur->next;
	}
	return (0);
}

int	ft_env(char **args)
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
