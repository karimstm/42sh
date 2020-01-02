/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 13:29:57 by amoutik           #+#    #+#             */
/*   Updated: 2019/10/12 11:43:43 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"

int     is_match(const char *pattern, const char *string);

static DIR    *g_opendir(char *str)
{
  char buf[MAXPATHLEN];

  if (!*str)
    ft_strcpy(buf, ".");
  else
    ft_strcpy(buf, str);
  return(opendir(buf));
}

t_list_path   *g_readdir(DIR *dirp, int flags, char *filename)
{
  t_list_path   *list_names;
  struct dirent *dp;

  list_names = alloc_path_list(1);
  if (dirp == NULL)
    return (NULL);
  while ((dp = readdir(dirp)) != NULL)
  {
    if (dp->d_name[0] != '.')
    {
      if (flags & ISDIR)
      {
        if (dp->d_type == DT_DIR)
          push_path(list_names, (ft_strcmp(filename, ".") ? ft_strjoin(filename, dp->d_name) : ft_strdup(dp->d_name)), ft_strlen(dp->d_name));
      }
      else
        push_path(list_names, (ft_strcmp(filename, ".") ? ft_strjoin(filename, dp->d_name) : ft_strdup(dp->d_name)), ft_strlen(dp->d_name));
    }
  }
  closedir(dirp);
  return (list_names);
}

void    _match_reg(char *to_find, t_list_path *list)
{
  t_path  *current;

  current = list->head;
  while (current)
  {
    if (is_match(to_find, &current->path[ft_strlen(current->path) - current->path_len]))
      delete_target(list, &current);
    if (current)
      current = current->next;
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

  current = list->head;
  while (current)
  {
    _glob_loop(current->path, pglob);
    delete_target(list, &current);
    if (current)
      current = current->next;
  }
}

void    _glob_match(char *dirname, char *pattern, t_glob *pglob)
{
  int flags;
  t_list_path *list_names;

  flags = 0;

  if (pattern[ft_strlen(pattern) - 1] == SEP)
    flags |= ISDIR;
  list_names  = g_readdir(g_opendir(dirname), flags, dirname);
  if (list_names == NULL)
  	return ;
  _match_reg(pattern, list_names);
  /* got up to the next SEP '/' */
  while (*pattern && *pattern != SEP)
    pattern++;
  if (*pattern != EOS)
    list_func(list_names, pattern, _join_with_path);
  /* loop back recursively on each element in the list */
  if (*pattern != EOS)
    if (!(*pattern == SEP && ft_strlen(pattern) == 1))
      _glob_back(list_names, pglob);
  list_names->head = merge_sort(list_names->head);
  //print_list(list_names);
  //printf("hi\n");
  fill_glob(list_names, pglob);
}

char    *_get_starting_path(char *path)
{
  size_t len;

  len = ft_strlen(path);
  while (len--)
  {
    if (path[len] == SEP)
      return (strndup(path, len + 1));
  }
  return (".");
}

char    *_basename(char *path)
{
  size_t len;

  len = ft_strlen(path);
  while (len--)
  {
    if (path[len] == SEP)
      return (strdup(&path[++len]));
  }
  return path;
}

/* Loop throught the pattern */

void    _glob_loop(char *pattern, t_glob *pglob)
{
  char  dirname[MAXPATHLEN];
  char  *basename;
  // char  prev_token;
  int   index;

  ft_bzero(dirname, MAXPATHLEN);
  // prev_token = 0;
  index = 0;
  while (*pattern)
  {
    if (*pattern == STAR || *pattern == QUESTION || *pattern == LBRACKET)
    {
      basename = _basename(dirname);
      return (_glob_match(_get_starting_path(dirname), ft_strjoin(basename, pattern), pglob));
      /* The third argument here is to denote a string to find */
    }
    // prev_token = *pattern;
    dirname[index++] = *pattern;
    pattern++;
  }
}

int     _glob(const char *pattern, int flags, int (*errfunc)(const char *epath, int errno), t_glob *pglob)
{
  if (pattern == NULL || pglob == NULL || (flags & ~__GLOB_FLAGS) != 0)
    return (-1);
  _glob_loop((char *)pattern, pglob);
  (void)pglob;
  (void)errfunc;
  return (0);
}
