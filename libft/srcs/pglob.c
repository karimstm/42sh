/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pglob.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 10:56:13 by amoutik           #+#    #+#             */
/*   Updated: 2020/03/02 14:24:25 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"

void	del_list(t_list_path *todel)
{
	t_path	*cur;
	t_path	*next;

	cur = todel ? todel->head : 0;
	while (cur)
	{
		next = cur->next;
		ft_strdel(&cur->path);
		free(cur);
		cur = next;
	}
	if (todel)
		free(todel);
}

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
	t_path		*current;
	int			i;
	char		**names;

	i = 0;
	names = init_glob((size_t)list->node_count + pglob->gl_pathc);
	while ((size_t)i < pglob->gl_pathc)
	{
		names[i] = pglob->gl_pathv[i];
		i++;
	}
	if (pglob->gl_pathv)
		free(pglob->gl_pathv);
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
	del_list(list);
}

void	ft_match_reg(char *to_find, t_list_path *list)
{
	t_path		*current;
	t_path		*next;

	current = list && list->head ? list->head : NULL;
	while (current)
	{
		next = current->next;
		if (is_match(to_find,
			&current->path[ft_strlen(current->path) - current->path_len]))
			delete_target(list, &current);
		current = next;
	}
}

char	*ft_join_with_path(char **s1, char *s2)
{
	char		*string;

	string = ft_strjoin(*s1, s2);
	free(*s1);
	return (string);
}
