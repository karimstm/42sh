/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 13:29:57 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/28 20:14:43 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"
#include "libft.h"

int     is_match(const char *pattern, const char *string);

static DIR    *g_opendir(char *str)
{
  char buf[MAXPATHLEN];

  if (str && !*str)
    ft_strcpy(buf, ".");
  else
    ft_strcpy(buf, str);
  return (opendir(buf));
}

t_list_path   *g_readdir(DIR *dirp, int flags, char *pattern, char *dirname)
{
  t_list_path   *list_names;
  struct dirent *dp;

  list_names = alloc_path_list(1);
  if (dirp == NULL)
  {
    ft_strdel(&dirname);
    free(list_names);
    return (NULL);
  }
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

void    _match_reg(char *to_find, t_list_path *list)
{
  t_path  *current;
  t_path  *next;
  
  current = list && list->head ? list->head : NULL;
  while (current)
  {
    next = current->next;  
    if (is_match(to_find, &current->path[ft_strlen(current->path) - current->path_len]))
      delete_target(list, &current);
    current = next;
  }
}

char    *_join_with_path(char **s1, char *s2)
{
  char *string;
  string = ft_strjoin(*s1, s2);
  free(*s1);
  return (string);
}

void    _glob_back(t_list_path *list, t_glob *pglob)
{
  t_path *current;
  t_path *next;
  
  current = list->head;
  while (current)
  {
    next = current->next;
    _glob_loop(current->path, pglob);
    delete_target(list, &current);
    current = next;
  }
}

void    _glob_match(char *dirname, char *pattern, t_glob *pglob)
{
  int flags;
  t_list_path *list_names;
  char *tmp;

  tmp = pattern;
  flags = 0;
  if (pattern[ft_strlen(pattern) - 1] == SEP)
    flags |= ISDIR;
  list_names  = g_readdir(g_opendir(dirname), flags, pattern, dirname);
  if (list_names == NULL)
  {
    ft_strdel(&tmp);
  	return ;
  }
  _match_reg(pattern, list_names);
  while (*pattern && *pattern != SEP)
    pattern++;
  if (*pattern != EOS)
    list_func(list_names, pattern, _join_with_path);
  if (*pattern != EOS)
    if (!(*pattern == SEP && ft_strlen(pattern) == 1))
      _glob_back(list_names, pglob);
  list_names->head = merge_sort(list_names->head);
  fill_glob(list_names, pglob);
  ft_strdel(&tmp);
}

char    *_get_starting_path(char *path)
{
  size_t len;
  
  len = ft_strlen(path);
  while (len)
  {
    if (path[len] == SEP)
      return (ft_strndup(path, len));
    len--;
  }
  return (ft_strdup("."));
}

char    *_basename(char *path)
{
  size_t len;

  len = ft_strlen(path);
  while (len--)
  {
    if (path[len] == SEP)
      return (ft_strdup(&path[++len]));
  }
  return ft_strdup(path);
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
void exec_glob(char *dirname, char *pattern, t_glob *pglob)
{
  char *basename;
  char *pathdir;
  char *tmp;

  tmp = pattern;
  basename = _basename(dirname);
  tmp = ft_strjoin(basename, pattern);
  pathdir = _get_starting_path(dirname);
  ft_strdel(&dirname);
  ft_strdel(&basename);
  return (_glob_match(pathdir, tmp, pglob));
}

void    _glob_loop(char *pattern, t_glob *pglob)
{
  char  *dirname;
  int   index;
  int   len;
  char *start;
  
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
    else if (*pattern == STAR || *pattern == QUESTION || *pattern == LBRACKET)
      return (exec_glob(dirname, pattern, pglob));
    dirname[index++] = *pattern;
    if(*pattern)
      pattern++;
  }
  ft_strdel(&dirname);
}

int     _glob(const char *pattern, int flags, int (*errfunc)(const char *epath, int errno), t_glob *pglob)
{
  ft_memset(pglob, 0, sizeof(pglob));
  pglob->gl_pathv = NULL;
  if (pattern == NULL || pglob == NULL || (flags & ~__GLOB_FLAGS) != 0)
    return (-1);
  _glob_loop((char *)pattern, pglob);
  (void)errfunc;
  return (0);
}

