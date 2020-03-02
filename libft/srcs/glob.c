/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 13:29:57 by amoutik           #+#    #+#             */
/*   Updated: 2020/03/02 11:02:46 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"
#include "libft.h"

static DIR			*g_opendir(char *str)
{
	char buf[MAXPATHLEN];

	if (str && !*str)
		ft_strcpy(buf, ".");
	else
		ft_strcpy(buf, str);
	return (opendir(buf));
}

void						*free_dir(char *dirname, t_list_path *list_names)
{
	ft_strdel(&dirname);
	free(list_names);
	return (NULL);
}

t_list_path			*g_readdir(DIR *dirp,
									int flags, char *pattern, char *dirname)
{
	t_list_path			*list_names;
	struct dirent		*dp;

	list_names = alloc_path_list(1);
	if (dirp == NULL)
		return ((t_list_path *)free_dir(dirname, list_names));
	while ((dp = readdir(dirp)))
	{
		if (!ft_strequ(dp->d_name, ".") && !ft_strequ(dp->d_name, ".."))
		{
			if (!(dp->d_name[0] == '.' && pattern[0] != '.'))
			{
				if (flags & ISDIR)
				{
					if (dp->d_type == DT_DIR)
						push_path(list_names, ft_strdup(dp->d_name), ft_strlen(dp->d_name));
				}
				else
					push_path(list_names, ft_strdup(dp->d_name), ft_strlen(dp->d_name));
			}
		}
	}
	closedir(dirp);
	ft_strdel(&dirname);
	return (list_names);
}

void		ft_match_reg(char *to_find, t_list_path *list)
{
	t_path		*current;
	t_path		*next;

	current = list && list->head ? list->head : NULL;
	while (current)
	{
		next = current->next;
		if (is_match(to_find, &current->path[ft_strlen(current->path) - current->path_len]))
			delete_target(list, &current);
		current = next;
	}
}

char		*ft_join_with_path(char **s1, char *s2)
{
	char		*string;

	string = ft_strjoin(*s1, s2);
	free(*s1);
	return (string);
}

void		ft_glob_back(t_list_path *list, t_glob *pglob)
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

void			ft_glob_match(char *dirname, char *pattern, t_glob *pglob)
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

char		*ft_get_starting_path(char *path)
{
	size_t	len;

	len = ft_strlen(path);
	while (len)
	{
		if (path[len] == SEP)
			return (ft_strndup(path, len));
		len--;
	}
	return (ft_strdup("."));
}

char		*ft_basename(char *path)
{
	size_t len;

	len = ft_strlen(path);
	while (len--)
	{
		if (path[len] == SEP)
			return (ft_strdup(&path[++len]));
	}
	return (ft_strdup(path));
}

int			skip_dquote(char **string, char c)
{
	char	*new;
	size_t	len;

	new = *string;
	new++;
	while (*new)
	{
		if (*new == '\\' && c != '\'')
			new++;
		else if (*new == c)
		{
			new++;
			break ;
		}
		*new ? new++ : 0;
	}
	len = new - *string;
	*string = new;
	return (len);
}

void			exec_glob(char *dirname, char *pattern, t_glob *pglob)
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

void			ft_glob_loop(char *pattern, t_glob *pglob)
{
	char		*dirname;
	int			index;
	int			len;
	char		*start;

	dirname = ft_memalloc(MAXPATHLEN);
	index = 0;
	while (*pattern)
	{
		if (*pattern == '\\')
			pattern++;
		else if (*pattern == '"' || *pattern == '\'')
		{
			start = pattern;
			len = skip_dquote(&pattern, *pattern);
			ft_strncpy(&dirname[index], start, len);
			index += len;
		}
		else if (*pattern == '*' || *pattern == '?' || *pattern == '[')
			return (exec_glob(dirname, pattern, pglob));
		dirname[index++] = *pattern;
		if (*pattern)
			pattern++;
	}
	ft_strdel(&dirname);
}

int				ft_glob(const char *pattern, int flags,
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
