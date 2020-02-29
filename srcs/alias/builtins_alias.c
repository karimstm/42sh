/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_alias.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 13:09:36 by cjamal            #+#    #+#             */
/*   Updated: 2020/02/29 13:19:07 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	check_option(char **args)
{
	int		found;
	int		i;
	char	*str;

	found = 1;
	i = 0;
	while (args[i])
	{
		str = args[i++];
		if (str && str[0] == '-')
		{
			str++;
			while (*str)
			{
				if (*str == 'a')
					found = 0;
				else
					return (print_invalid_option(*str));
				str++;
			}
		}
	}
	return (found);
}

int	print_usage(void)
{
	dprintf(2, "unalias: usage: unalias [-a] name [name ...]\n");
	return (2);
}

int	print_invalid_option(char c)
{
	dprintf(2, "42sh: unalias: -%c: invalid option\n", c);
	return (print_usage());
}

int	ft_alias(char **args)
{
	t_alias_list	*alias_list;
	t_alias			*alias;

	alias_list = get_alias_list(NULL);
	DECLARE(int, _(error, 0), _(i, -1));
	if (args == NULL || !*args)
		print_aliases(alias_list);
	else
		while (args[++i])
		{
			if (is_alias_pattern(args[i]))
				alias_insert(alias_list, args[i]);
			else
			{
				if (!(alias = alias_find(alias_list, args[i])))
				{
					ft_printf_fd(2, "42sh: alias: %s: not found\n", args[i]);
					error = 1;
				}
				else
					ft_printf_fd(2, "alias: %s='%s'\n",
						alias->alias, alias->value);
			}
		}
	return (error);
}

int	ft_unalias(char **args)
{
	int				i;
	t_alias_list	*list;

	ft_putendl("TEST");
	list = get_alias_list(NULL);
	i = 0;
	if (args == NULL || *args == NULL)
		return (print_usage());
	else if (list)
	{
		if (!check_option(args))
			aliase_prune(list);
		else
		{
			while (args[i])
			{
				delete_alias(list, args[i]);
				i++;
			}
		}
	}
	return (0);
}
