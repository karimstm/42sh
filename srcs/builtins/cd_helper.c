/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_helper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/28 19:19:20 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/27 00:39:22 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int is_correct_path(char *path, int print_error)
{
	if (path && !access(path, F_OK))
	{
		if (!is_directory(path))
		{
			if (print_error)
				PRINT_ERROR2(path, " Not a directory");
			return (0);
		}
		return (1);
	}
	if (print_error)
		PRINT_ERROR2(path, "No such file or directory");
	return (0);
}

char *get_path_var(const char *key, int get_current_dir)
{
	t_variables *var;

	if ((var = get_var((char *)key)))
		return (ft_strdup(var->value));
	if (get_current_dir)
		return (getcwd(NULL, 0));
	return (NULL);
}

char *concat_path_with_cdpath(char *path)
{
	char *tmp;
	char **cd_paths;
	t_variables *var;
	int i;

	i = 0;
	var = get_var("CDPATH");
	cd_paths = ft_strsplit(var->value ? var->value : NULL, ' ');
	if (cd_paths)
	{
		while (cd_paths)
		{
			tmp = ft_strnjoin((char *[]){cd_paths[i], "/", path}, 3);
			if (is_correct_path(tmp, 0))
			{
				free(path);
				path = tmp;
				break;
			}
			free(tmp);
		}
		ft_free_strtab(cd_paths);
	}
	return (path);
}