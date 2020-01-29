/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/25 23:20:37 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/01/29 01:56:30 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	ft_get_cwd(char *ret, char *path)
{
	char	*tmp;
	char	*tmp2;
	int		diff;

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
		path = get_home_path();
	else if (ft_strequ(path, "-"))
		path = get_oldpwd();
	if (path && *path && !ft_isinstr(*path, "./"))
		path = concat_path_with_cdpath();
	if (path && *path && *path != '/')
	{
		if((pwd = get_pwd()))
		{
			tmp = path;
			path = ft_strnjoin((char*[]){pwd, "/", path});
			free(tmp);
		}
	}
	if(!is_correct_path(path))
		ft_strdel(&path);
	return (path);
}

int		ft_cd(char **cmd)
{
	char *path;
	int logicaly;

	logicaly = get_cd_flag();
	path = get_cd_path(cmd);
	if (path)
	{
		if (logicaly)
			ft_get_cwd(path, path + 2);
		if (!chdir(path))
		{
			if(!logicaly)
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
