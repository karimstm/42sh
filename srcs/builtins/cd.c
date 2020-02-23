/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/25 23:20:37 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/23 01:57:43 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int get_cd_flag(char ***cmd)
{
	char **tmp;
	int ret;
	int i;

	ret = 1;
	tmp = *cmd;
	while (*cmd)
	{
		if (ft_strequ(*tmp, "-") || **tmp != '-')
			break;
		i = 0;
		while (tmp[0][++i])
		{
			if(tmp[0][i] != 'L' && tmp[0][i] != 'P')
				return(ft_printf_fd(2, "cd: usage: cd [-L|-P] [dir]") * 0 - 1);
			ret = tmp[0][i] == 'L';
		}
		tmp++;
	}
	*cmd = tmp;
	return(ret);
}

void ft_get_cwd(char *ret, char *path)
{
	char *tmp;
	char *tmp2;
	int diff;

	*ret = 0;
	while (*path)
	{
		if ((tmp = ft_strchr(path, '/')) ||
			(tmp = ft_strchr(path, '\0')))
		{
			if ((diff = (tmp - path)) == 2 && ft_strnequ(path, "..", 2))
			{
				if (*ret && (tmp2 = ft_strrchr(ret, '/')))
					*tmp2 = 0;
			}
			else if (diff && *path != '.')
			{
				if ((tmp2 = ft_strchr(ret, 0)) && (!*ret || tmp2[-1] != '/'))
					ft_strcat(tmp2, "/");
				ft_strncat(tmp2 + 1, path, diff);
			}
			path += diff + !!*tmp;
		}
	}
	!*ret ? ft_strcat(ret, "/") : 0;
}

int get_cd_path(char **cmd)
{
	char *path;
	char *pwd;
	char *tmp;

	path = cmd[1];
	if (!path)
		path = get_path_var("HOME", 0);
	else if (ft_strequ(path, "-"))
		path = get_path_var("OLDPWD", 0);
	if (path && *path && !ft_isinstr(*path, "./"))
		path = concat_path_with_cdpath(path);
	if (path && *path && *path != '/' && (pwd = get_path_var("PWD", 1)))
	{
		tmp = path;
		path = ft_strnjoin((char *[]){pwd, "/", path}, 3);
		free(tmp);
		free(pwd);
	}
	if (!is_correct_path(path))
		ft_strdel(&path);
	return (path);
}

int ft_cd(char **cmd)
{
	char *path;
	int logicaly;

	if ((logicaly = get_cd_flag(&cmd)) == -1)
		return(-1);
	path = get_cd_path(cmd);
	if (path)
	{
		if (logicaly)
			ft_get_cwd(path, path + 2);
		if (!chdir(path))
		{
			if (!logicaly)
			{
				free(path);
				path = getcwd(NULL, 0);
			}
			ft_update_pwd(path, var);
			free(path);
			return (0);
		}
		free(path);
		return (PRINT_ERROR(dir, PERM_DENYD));
	}
}
