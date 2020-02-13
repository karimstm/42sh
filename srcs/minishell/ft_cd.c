/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/02 11:50:18 by zoulhafi          #+#    #+#             */
/*   Updated: 2020/02/12 18:52:35 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
**	this function called by ft_cd function,
**	it's change the value of the environment variable
**	$PWD or $OLDPWD given as parameter char *pwd,
**	if this variable not found it creates a new one.
*/

static void	change_pwd(char *pwd)
{
	t_variables	*cpy;
	char	*new_home;
	
	cpy = get_var(pwd);
	if (cpy)
	{
		ft_strdel(&cpy->value);
		cpy->value = getcwd(cpy->value, 0);
		cpy->is_modified = ENV_MODIFIED;
		return ;
	}
	new_home = NULL;
	variable_push(pwd, getcwd(new_home, 0), 1, ENV_ADDED);
	ft_strdel(&new_home);
}

/*
**	called by ft_cd function,
**	it checks if the string home given as parameter
**	is a file or a directory and exist or not and it's permission.
*/

static int	check_dir(char *home)
{
	struct stat	st;
	int			ret;

	ret = -1;
	if (home == NULL)
		ret = ft_printf_fd(2, "$HOME variable not found (check env).\n");
	else if (stat(home, &st) != 0)
		ret = ft_printf_fd(2, "%s: No such file or directory.\n", home);
	else if (home != NULL && access(home, F_OK) != 0)
		ret = ft_printf_fd(2, "%s: No such file or directory.\n", home);
	else if (home != NULL && (st.st_mode & S_IFMT) != S_IFDIR)
		ret = ft_printf_fd(2, "%s: Not a directory.\n", home);
	else if (home != NULL && access(home, X_OK) != 0)
		ret = ft_printf_fd(2, "%s: Permission denied.\n", home);
	if (ret != -1)
		return (-1);
	else
		return (1);
}

/*
**	a builtin command, that move the current location to another one,
**	it changes the value of $PWD and $OLDPWD, and calls chdir() function,
**	it checks if the new location exists and it's permission
**	before it makes any change.
*/

int			ft_cd(char **args)
{
	char		*home;
	char		*new;
	char		*oldpwd;
	t_variables	*env;

	new = NULL;
	if (*args == NULL)
	{
		env = get_var("HOME");
		home = env ? env->value : NULL;
	}
	else
		home = *args;
	if (home != NULL && ft_strcmp(home, "-") == 0)
	{
		env = get_var("OLDPWD");
		oldpwd = env ? env->value : NULL;
		new = oldpwd ? ft_strdup(oldpwd) : ft_strdup(".");
	}
	if (new != NULL)
		home = new;
	if (check_dir(home) == 1)
	{
		change_pwd("OLDPWD");
		chdir(home);
		change_pwd("PWD");
	}
	else
		return (1);

	if (new != NULL)
		free(new);
	return (0);
}
