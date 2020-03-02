/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 13:29:57 by amoutik           #+#    #+#             */
/*   Updated: 2020/03/02 14:34:17 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"

static DIR			*g_opendir(char *str)
{
	char buf[MAXPATHLEN];

	if (str && !*str)
		ft_strcpy(buf, ".");
	else
		ft_strcpy(buf, str);
	return (opendir(buf));
}

void				ft_glob_back(t_list_path *list, t_glob *pglob)
{
	t_path	*current;
	t_path	*next;

	current = list->head;
	while (current)
	{
		next = current->next;
		ft_glob_loop(current->path, pglob);
		delete_target(list, &current);
		current = next;
	}
}

void				ft_glob_match(char *dirname, char *pattern, t_glob *pglob)
{
	t_list_path				*list_names;
	int						flags;
	char					*tmp;

	tmp = pattern;
	flags = 0;
	if (pattern[ft_strlen(pattern) - 1] == SEP)
		flags |= ISDIR;
	list_names = g_readdir(g_opendir(dirname), flags, pattern, dirname);
	if (list_names == NULL)
	{
		ft_strdel(&tmp);
		return ;
	}
	ft_match_reg(pattern, list_names);
	while (*pattern && *pattern != SEP)
		pattern++;
	if (*pattern != EOS)
		list_func(list_names, pattern, ft_join_with_path);
	if (*pattern != EOS)
		if (!(*pattern == SEP && ft_strlen(pattern) == 1))
			ft_glob_back(list_names, pglob);
	list_names->head = merge_sort(list_names->head);
	fill_glob(list_names, pglob);
	ft_strdel(&tmp);
}

void				exec_glob(char *dirname, char *pattern, t_glob *pglob)
{
	char		*basename;
	char		*pathdir;
	char		*tmp;

	tmp = pattern;
	basename = ft_basename(dirname);
	tmp = ft_strjoin(basename, pattern);
	pathdir = ft_get_starting_path(dirname);
	ft_strdel(&dirname);
	ft_strdel(&basename);
	return (ft_glob_match(pathdir, tmp, pglob));
}

int					ft_glob(const char *pattern, int flags,
						int (*errfunc)(const char *epath, int errno),
							t_glob *pglob)
{
	ft_memset(pglob, 0, sizeof(pglob));
	pglob->gl_pathv = NULL;
	if (pattern == NULL || pglob == NULL || (flags & ~__GLOB_FLAGS) != 0)
		return (-1);
	ft_glob_loop((char *)pattern, pglob);
	(void)errfunc;
	return (0);
}
