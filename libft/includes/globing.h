/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/26 15:03:57 by amoutik           #+#    #+#             */
/*   Updated: 2019/12/30 11:27:09 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBING_H
#define GLOBING_H

#include <glob.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fnmatch.h>
#include "libft.h"
#include "globing.h"

#define DOLLAR      '$'
#define DOT         '.'
#define EOS         '\0'
#define LBRACKET    '['
#define NOT         '!'
#define QUESTION    '?'
#define QUOTE       '\\'
#define RANGE       '-'
#define RBRACKET    ']'
#define SEP         '/'
#define STAR        '*'
#define TILDE       '~'
#define UNDERSCORE  '_'
#define LBRACE      '{'
#define RBRACE      '}'
#define SLASH       '/'
#define COMMA       ','

# define MAXPATHLEN 4096
# define M_ISDIR(m) ((m & 0170000) == 0040000)
# define ISDIR 1
# define __GLOB_FLAGS (GLOB_ERR|GLOB_MARK|GLOB_NOSORT|GLOB_DOOFFS \
		| GLOB_NOESCAPE|GLOB_NOCHECK|GLOB_APPEND)

typedef	struct 
{
  size_t	gl_pathc;		/* count of total paths so far */
  size_t	gl_matchc; 		/* count of paths matching pattern */
  size_t	gl_offs;		/* reserved at beginning of gl_pathv */
  int		gl_flags; 		/* returned falgs */
  char		**gl_pathv; 	/* list of paths matching pattern */
}			t_glob;

typedef struct 		s_path
{
  char			*path;	/* stores the path or part of it */
  size_t    path_len;
  struct s_path	*next;
  struct s_path	*prev;
}					t_path;

typedef	struct		s_list_path
{
  struct s_path	*head;
  struct s_path	*tail;
  int				node_count;
}					t_list_path;

/*
 ** list.c
 */

/*
 ** allocate node list to start storing in it;
 */

t_list_path		*alloc_path_list(size_t size);
void			push_path(t_list_path *ptr, char *name, size_t len);
void			print_list(t_list_path *list);
void			free_list_path(t_list_path *list);
void			delete_target(t_list_path *list, t_path **current);
void      list_func(t_list_path *list, char *s1, char * (f)(char **, char *));
void      init_s_path(t_list_path *list);

/*
** glob.c
*/

int     _glob(const char *pattern, int flags, int (*errfunc)(const char *epath, int errno), t_glob *pglob);
void    _glob_loop(char *expr, t_glob *pglob);

/*
**  ft_strcmp_q.c
*/

int   ft_strcmp_q(const char *s1, const char *s2);

/*
**  ft_match.c
*/

int   is_match(const char *pattern, const char *string);

/*
** error.c
*/

int   error_func(const char *epath, int errno);

/*
** merge.c
*/

t_path	*merge_sort(t_path *head);


/*
** pglob.c
*/

void	fill_glob(t_list_path *list, t_glob *pglob);

#endif
