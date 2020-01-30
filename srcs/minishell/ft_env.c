/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 21:26:42 by zoulhafi          #+#    #+#             */
/*   Updated: 2020/01/30 12:16:29 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
**	needed by many functions,
**	it get the variabled named with char *name given on parameter
**	from the list t_list that contains all t_env elements.
*/

char		*get_env_value(char *name, t_list *lst)
{
	t_env	*env;

	while (lst)
	{
		env = (t_env*)lst->content;
		if (ft_strcmp(env->name, name) == 0)
			return (env->value);
		lst = lst->next;
	}
	return (NULL);
}

/*
**	check usage of setenv builtin command.
*/

static int	check_setenv_args(char **args, t_list **lst)
{
	if (ft_strtablen(args) > 2)
	{
		ft_printf_fd(2, "setenv: Too many arguments.\n");
		return (-1);
	}
	if (*args == NULL)
	{
		ft_env(args, lst);
		return (-1);
	}
	if (ft_isalpha(**args) == 0)
	{
		ft_printf_fd(2, "setenv: Variable name must begin with a letter.\n");
		return (-1);
	}
	else if (ft_str_isalnum(*args) == 0)
	{
		ft_printf_fd(2, "setenv: Variable name must contain ");
		ft_printf_fd(2, "alphanumeric characters.\n");
		return (-1);
	}
	return (1);
}

/*
**	a builtin command, it add new environment variable
**	to the list t_list that contains t_env elements.
*/

int		ft_setenv(char **args, t_list **lst)
{
	t_list	*cpy;
	t_env	*env;

	if (check_setenv_args(args, lst) == -1)
		return (1);
	cpy = *lst;
	while (cpy)
	{
		env = (t_env*)cpy->content;
		if (ft_strcmp(env->name, *args) == 0)
		{
			free(env->value);
			env->value = args[1] ? ft_strdup(args[1]) : ft_strdup("");
			return (0);
		}
		cpy = cpy->next;
	}
	add_env(lst, args[0], args[1], 1);
	return (0);
}

/*
**	a builtin command, it removes all variables stored in **args
**	from the list t_list that contains t_env elements.
*/

int		ft_unsetenv(char **args, t_list **lst)
{
	t_list	*cpy;
	t_list	*previous;
	int		i;

	i = -1;
	while (args[++i] != NULL)
	{
		cpy = *lst;
		previous = NULL;
		while (cpy != NULL)
		{
			if (ft_strcmp(((t_env*)cpy->content)->name, args[i]) == 0)
			{
				if (previous == NULL)
					*lst = cpy->next;
				else
					previous->next = cpy->next;
				free_elem_env(cpy);
				break ;
			}
			previous = cpy;
			cpy = cpy->next;
		}
	}
	return (0);
}

/*
**	a builtin command, it prints all environment variables
*/

int		ft_env(char **args, t_list **lst)
{
	t_list	*cpy;
	t_env	*env;

	(void)args;
	cpy = *lst;
	while (cpy)
	{
		env = (t_env*)cpy->content;
		ft_printf("%s=%s\n", env->name, env->value);
		cpy = cpy->next;
	}
	return (0);
}
