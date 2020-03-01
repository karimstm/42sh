/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szakaria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/01 04:20:00 by szakaria          #+#    #+#             */
/*   Updated: 2020/03/01 06:22:12 by szakaria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "auto_compilation.h"

char			**stock_path_access(int j, char *line)
{
	DIR				*dir;
	char			**stock;
	struct dirent	*sd;

	stock = (char **)malloc(sizeof(char *) * (j + 1));
	stock[j] = NULL;
	j = 0;
	if (ft_strlen(line))
		dir = opendir(line);
	else
		dir = opendir(".");
	if (dir)
		while ((sd = readdir(dir)) != NULL)
			if (line && (sd->d_name[0] != '.' || ft_strchr(line, '.')))
				stock[j++] = ft_strdup(sd->d_name);
	if (dir)
		closedir(dir);
	return (stock);
}

char			**context_type_path(char *line)
{
	DIR				*dir;
	struct dirent	*sd;
	int				cont;

	cont = 0;
	if (ft_strlen(line) && access(line, F_OK) != 0)
		return (NULL);
	if (ft_strlen(line))
		dir = opendir(line);
	else
		dir = opendir(".");
	if (dir)
		while ((sd = readdir(dir)) != NULL)
			if (line && (sd->d_name[0] != '.' || ft_strchr(line, '.')))
				cont++;
	if (dir)
		closedir(dir);
	return (cont ? stock_path_access(cont, line) : NULL);
}
