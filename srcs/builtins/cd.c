/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/25 23:20:37 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/03/02 16:35:24 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	ft_update_pwd(char *path)
{
	t_variables	*pwd;

	pwd = get_var("PWD");
	if (pwd)
		edit_add_var("OLDPWD", pwd->value, 0, 1);
	edit_add_var("PWD", path, 0, 1);
	ft_printf("%s\n", path);
}

int		get_cd_flag(char ***cmd)
{
	char	**tmp;
	int		ret;
	int		i;

	ret = 1;
	tmp = *cmd;
	while (*tmp)
	{
		if (ft_strequ(*tmp, "-") || **tmp != '-')
			break ;
		i = 0;
		while (tmp[0][++i])
		{
			if (tmp[0][i] != 'L' && tmp[0][i] != 'P')
				return (PRINT_ERROR2("cd", "usage: cd [-L|-P] [dir]") * 0 - 1);
			ret = tmp[0][i] == 'L';
		}
		tmp++;
	}
	*cmd = tmp;
	return (ret);
}

char	*ft_get_cwd(char *path, char *ret, char *tmp, char *tmp2)
{
	const char	*ptr = path;
	int			diff;

	if (ret && !(*ret = 0))
		while (*path)
			if ((tmp = ft_skip_unitl_char(path, "/", NULL)))
			{
				if ((diff = (tmp - path)) == 2 && ft_strnequ(path, "..", 2))
				{
					if (*ret && (tmp2 = ft_strrchr(ret, '/')))
						*tmp2 = 0;
				}
				else if (diff && (diff != 1 || *path != '.'))
				{
					if ((tmp2 = ft_strchr(ret, 0)) && (!*ret || tmp2[-1] != 47))
						ft_strcat(tmp2, "/");
					ft_strncat(tmp2 + 1, path, diff);
				}
				path += diff + !!*tmp;
			}
	free((void *)ptr);
	return (ret && !*ret ? ft_strcat(ret, "/") : ret);
}

char	*get_cd_path(char *path)
{
	char	*pwd;
	char	*tmp;

	if (!path)
		path = get_path_var("HOME", 0);
	else if (ft_strequ(path, "-"))
	{
		free(path);
		path = get_path_var("OLDPWD", 0);
	}
	if (path && *path && !ft_isinstr(*path, "./"))
		path = concat_path_with_cdpath(path);
	if (path && *path && *path != '/' && (pwd = get_path_var("PWD", 1)))
	{
		tmp = path;
		path = ft_strnjoin((char *[]){pwd, "/", path}, 3);
		free(tmp);
		free(pwd);
	}
	if (!is_correct_path(path, 1))
		ft_strdel(&path);
	return (path);
}

int		ft_cd(char **cmd)
{
	char	*path;
	int		logicaly;

	if ((logicaly = get_cd_flag(&cmd)) == -1 || (cmd[0] && cmd[1]))
		return (cmd[1] ? PRINT_ERROR2("cd", "to many argument") : -1);
	path = get_cd_path(ft_strdup(*cmd));
	if (path)
	{
		if (logicaly)
			path = ft_get_cwd(path, ft_strdup(path), NULL, NULL);
		if (!chdir(path))
		{
			if (!logicaly)
				free(path);
			if (!logicaly)
				path = getcwd(NULL, 0);
			ft_update_pwd(path);
			free(path);
			return (0);
		}
		free(path);
		PRINT_ERROR2("cd", "permission denied");
		return (15);
	}
	return (1);
}
