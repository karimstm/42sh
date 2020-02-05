/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 15:52:28 by zoulhafi          #+#    #+#             */
/*   Updated: 2020/02/04 15:37:43 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
**	a builtin command, that print the current location of the shell,
**	there two options to get that value (current location),
**	first it checks the environment variables if there is a $PWD variable,
**	if it exists, it will print it's value and the function will be stopped,
**	otherwise, we have an autorised function getcwd that do the same thing.
*/

int		ft_pwd(char **args)
{
	char		*current_pwd;
	t_variables	*pwd;

	(void)args;
	pwd = get_var("PWD");
	current_pwd = NULL;
	ft_printf("%s\n", pwd ? pwd->value : getcwd(current_pwd, 0));
	ft_strdel(&current_pwd);
	return (0);
}
