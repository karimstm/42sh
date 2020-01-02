/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/26 14:49:24 by amoutik           #+#    #+#             */
/*   Updated: 2019/04/02 16:14:32 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"
/* global varaibale to be removed later on */

t_list_path   pathes;
/* ======================================== */

int		error_function(const char *epath, int errno)
{
  fprintf(stderr, "There is an error with this file : %s\nand the number of error is %d\n", epath, errno);
  return (errno);
}

t_list_path		*_opendir(char *filename, int flag)
{
  t_list_path	*list_names;
  DIR				*dirp;
  struct	dirent	*dp;

  list_names = alloc_path_list(1);
  dirp = opendir(filename);
  if (dirp == NULL)
    return (NULL);
  while ((dp = readdir(dirp)) != NULL)
  {
    if (dp->d_name[0] != '.')
    {
      if (flag & ISDIR)
      {
        if (dp->d_type == DT_DIR)
          push_path(list_names, ft_strcmp(filename, ".") ? ft_strjoin(filename, dp->d_name) : ft_strdup(dp->d_name));
      }
      else
          push_path(list_names, ft_strcmp(filename, ".") ? ft_strjoin(filename, dp->d_name) : ft_strdup(dp->d_name));
    }
  }
  closedir(dirp);
  free(filename);
  return (list_names);
}


void	_glob_match(t_list_path *list, char *find, int is_ending)
{
  t_path	*current;
  size_t  find_len;

  find_len = ft_strlen(find);
  current = list->head;
  while (current)
  {
    //printf("current is %s, indexed %s,  and find is %s\n", current->path,  &current->path[ft_strlen(current->path) - find_len], find);
    if (is_ending && ft_strcmp(find, &current->path[ft_strlen(current->path) - find_len]))
      delete_target(list, &current);
    if (!is_ending && !ft_strstr(current->path, find))
      delete_target(list, &current);
    if (current)
      current = current->next;
  }	
}	

int   last_del(char *str, char del)
{
  int len;

  len = ft_strlen(str);
  if (len == 0)
    return (0);
  while (--len && str[len] != del)
    ;
  if (len && str[len] == del)
    return (++len);
  return (len);
}

char    *_join_with_path(char **s1, char *s2)
{
    char *string;

    string = ft_strjoin(*s1, s2);
    free(*s1);
    return (string);
}

int   _is_dir(char *str)
{
    while (*str)
      if (*str++ == SEP)
        return (1);
    return (0);
}

void  _glop_recursive(t_list_path *list)
{
    t_path *current;
    struct  stat  buf; 
    current = list->head;
    while (current)
    {
      /* remember to check the lstat of each file in an "else if" statement */
      if (ft_strchr(current->path, STAR))
        _glob_loop(current->path);
      if (ft_strchr(current->path, QUESTION))
        _glob_loop(current->path);
      if (stat(current->path, &buf) == -1)
      {
        current = current->next;
        continue;
      }
      else
        push_path(&pathes, current->path);
      current = current->next;
    }
}

void  _match_til_del(t_list_path *list, char **expr)
{
  char    *start;
  char    *str;

  str = *expr;
  start = str;
  while (*str && *str != SEP)
  {
    if (*str == STAR)
    {
      start[str - start] = '\0';
      _glob_match(list, start, 0);
      start = ++str;
      continue;
    }
    str++;
  }
  if ((*str && *str == SEP) || *str == '\0')
  {
    start = strndup(start, str - start);
    _glob_match(list, start, 1);
  }
  *expr = str;
}

void  _loop_quest(t_list_path *list, char *str)
{
    t_path *current;

    current = list->head;
    while (current)
    {
      if (ft_strcmp_q(current->path, str) != 0)
        delete_target(list, &current);
      if (current)
        current = current->next;
    }
}

void	_glob_question(char *path, char *start, char	**expr)
{
    t_list_path   *list_names;
    int           len;
    int           path_len;
    int           flag;

    len = 0;
    path_len = ft_strlen(path);
    flag = 0;
    list_names = NULL;
    (*expr)++;
    if (path[0] == EOS)
    {
      if (!(list_names = _opendir(strdup("."), flag)))
        return ;
    }
    else
    {
      if (start[ft_strlen(start) - 1] == SEP)
        flag |= ISDIR;
      else if (_is_dir(start))
        flag |= ISDIR;
      len = last_del(path, SEP);
      if (!(list_names = _opendir(strndup(path, len), flag)))
        return ;
    }
    _glob_match(list_names, strndup(&path[len], path_len - len), 0);
    if (list_names == NULL)
      return ;
    _loop_quest(list_names, start);
    while (*(*expr) && *(*expr) != SEP)
      (*expr)++;
    list_func(list_names, *expr, _join_with_path);
    _glop_recursive(list_names);
}

void  _glob_astr(char *path, char **expr)
{
  t_list_path *list_names;
  int         len;
  int         flag;
  int         path_len;

  len = 0;
  path_len = ft_strlen(path);
  flag = 0;
  list_names = NULL;
  (*expr)++;
  if (path[0] == EOS)
  {
    if (!(list_names = _opendir(strdup("."), flag)))
      return ;
  }
  else
  {
    /*
    ** finding the first  '/' before '*'
    ** if it ends with '/' it a directory else it's a file
    */
    if ((*expr)[ft_strlen((*expr)) - 1] == SEP)
      flag |= ISDIR;
    else if (_is_dir(*expr))
      flag |= ISDIR;
    len = last_del(path, SEP);
    if (!(list_names = _opendir(strndup(path, len), flag)))
      return ;
  }
  _glob_match(list_names, strndup(&path[len], path_len - len), 0);
  while (!ft_isalnum(*(*expr)) && *(*expr) != SEP)
    (*expr)++;
  if (list_names == NULL)
    return ;
  _match_til_del(list_names, &(*expr));
  list_func(list_names, *expr, _join_with_path);
  _glop_recursive(list_names);
  /* 
   * send for recursive call on each path we found so far
   * and join them with the rest of the string patter (*expr)
   */
  return ;
}

int   _is_valid_bracket(char *str)
{
  int   flag;
  flag = 0;
  while (*str)
  {
    if (*str == '[')
      flag = 1;
    if (*str == ']' && flag)
      return (1);
    str++;
  }
  return (0);
}

void	_glob_brackets(char *path, char *expr)
{
  printf("%s\n", expr);
}

void	_glob_loop(char *expr)
{
  char	*start;
  char	path[MAXPATHLEN];
  int		i;

  i = 0;
  start = expr;
  ft_bzero(path, MAXPATHLEN);
  while (*expr)
  {
    if (*expr == '*')
      return (_glob_astr(path, &expr));
    else if (*expr == '?')
      return (_glob_question(path, start, &expr));
    else if (*expr == '[')
    {
      if (_is_valid_bracket(expr))
        _glob_brackets(expr);
    }
    else
      path[i++] = *expr;
    expr++;
  }
}

int		_glob(const char *pattern, int flags, int (*errfunc)(const char *epath, int errno), t_glob *pglob)
{
  if (pattern == NULL || pglob == NULL || (flags & ~__GLOB_FLAGS) != 0)
    return (-1);
  _glob_loop((char *)pattern);
  (void)pglob;
  (void)errfunc;
  return (0);
}


int		main(int argc, char **argv)
{
  t_glob	g;

  if (argc == 1)
    return (1);
  init_s_path(&pathes);
   _glob(argv[1], GLOB_DOOFFS, &error_function, &g);
  print_list(&pathes);
  return (0);
}
