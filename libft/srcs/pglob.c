/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pglob.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 10:56:13 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/28 19:00:15 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"

void    del_list(t_list_path *todel)
{
  t_path *cur;
  t_path *next;
  
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
	t_path *current;
	int i;
	char **names;
	
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
