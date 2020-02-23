/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_helper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/28 19:19:20 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/23 01:40:14 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int is_correct_path(char *path)
{
	if (path && !access(path + 1, F_OK))
	{
		if (ft_is_dir(path))
		{
			PRINT_ERROR(path, NOT_DIR);
			return (0);
		}
		return (1);
	}
	PRINT_ERROR(path, FILE_NOTFOUND);
	return (0);
}

char *get_path_var(const char *key, int get_current_dir)
{
	t_variables *var;

	if ((var = get_var("PWD")))
		return (ft_strdup(var->value));
	if (get_current_dir)
		return (getcwd(NULL, 0));
	return (NULL);
}

char *concat_path_with_cdpath(char *path)
{
	char *tmp;
	char **cd_paths;
	int i;

	i = 0;
	cd_paths = ft_strsplit(get_path_var("CDPATH", 0), ' ');
	if (cd_paths)
	{
		while (cd_paths && *cd_paths[i])
		{
			tmp = ft_strnjoin((char *[]){cd_paths[i], "/", path}, 3);
			if (is_correct_path(tmp))
			{
				free(path);
				path = tmp;
				break;
			}
			free(tmp);
		}
		free_2d_tab(cd_paths);
	}
	return (path);
}

void ft_update_pwd(char *path, t_env_var *var)
{
	const char *pwd = var->pwd->content;

	ft_lstmodifone(var->oldpwd, ft_strjoin("OLDPWD=", (pwd ? pwd + 4 : "")));
	ft_lstmodifone(var->pwd, ft_strjoin("PWD=", path));
	ft_strreplace(var->oldpwd->content, '=', 0);
	ft_strreplace(var->pwd->content, '=', 0);
}
