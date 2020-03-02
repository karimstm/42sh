/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context_bin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/01 04:18:07 by szakaria          #+#    #+#             */
/*   Updated: 2020/03/02 17:10:40 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "auto_compilation.h"

char		**ft_stock0(int i, char **path, char *line)
{
	DIR				*dir;
	char			**stock;
	struct dirent	*sd;
	int				j;

	stock = (char **)malloc(sizeof(char *) * (i + 1));
	stock[i] = NULL;
	i = 0;
	j = 0;
	while (path[i])
	{
		if ((dir = opendir(path[i])))
			while ((sd = readdir(dir)) != NULL)
				if (sd->d_name[0] != '.' &&
						!ft_strncmp(sd->d_name, line, ft_strlen(line)))
					stock[j++] = ft_strdup(sd->d_name);
		if (dir)
			closedir(dir);
		i++;
	}
	free_tab(path);
	return (stock);
}

char		**check_bin(char *line, char **path)
{
	DIR				*dir;
	struct dirent	*sd;
	int				i;
	int				cont;

	i = 0;
	cont = 0;
	while (path[i])
	{
		if ((dir = opendir(path[i])))
			while ((sd = readdir(dir)) != NULL)
				if (sd->d_name[0] != '.' &&
						!ft_strncmp(sd->d_name, line, ft_strlen(line)))
					cont++;
		if (dir)
			closedir(dir);
		i++;
	}
	if (cont)
		return (ft_stock0(cont, path, line));
	free_tab(path);
	return (NULL);
}

char		**context_type_bin(char *line)
{
	t_variables			*head;
	char				*valeur;

	head = g_env->head;
	while (head)
	{
		if (!ft_strncmp(head->key, "PATH", 4))
			valeur = head->value;
		head = head->next;
	}
	if (valeur)
		return (check_bin(line, ft_strsplit(valeur, ':')));
	return (NULL);
}
