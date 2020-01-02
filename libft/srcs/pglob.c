/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pglob.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 10:56:13 by amoutik           #+#    #+#             */
/*   Updated: 2019/10/12 11:42:42 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"

char	**init_glob(size_t size)
{
	char **names;

	names = (char **)malloc(sizeof(char *) * (size + 1));
	if (names == NULL)
		return (NULL);
	return (names);	
}


void	fill_glob(t_list_path *list, t_glob *pglob)
{
	t_path *current;
	int i;

	i = 0;
	char **names = init_glob((size_t)list->node_count + pglob->gl_pathc);
	while ((size_t)i < pglob->gl_pathc)
	{
		names[i] = pglob->gl_pathv[i];
		i++;
	}
	current = list->head;
	while (current)
	{
		names[i] = ft_strdup(current->path);
		pglob->gl_pathc++;
		i++;
		current = current->next;
	}
	pglob->gl_pathv = names;
	pglob->gl_pathv[i] = NULL;
}
