/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auto_compilation.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/01 04:22:35 by szakaria          #+#    #+#             */
/*   Updated: 2020/03/02 17:10:40 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTO_COMPILATION_H
# define AUTO_COMPILATION_H
# define _(a, b) a = b
# define DECLARE(type, ...) type __VA_ARGS__
# include "../ft_readline.h"
# include <stdio.h>
# include <dirent.h>
# include <stdlib.h>
# include <termios.h>
# define EXIT 27

/*
**				affichage element selcted
*/

char					*print_window_element(char **stock, int i);

/*
**				Context -> type : variables | builtins | PATH
*/

char					**context_type_bin(char *line);
char					**context_type_path(char *line);
char					**context_type_var(char *new_line);

/*
**				FREE tab 2 montion
*/

void					free_tab(char **stock);

/*
** 					Var Global
*/

typedef struct			s_variables
{
	char				*key;
	char				*value;
	int					is_exported;
	int					is_modified;
	struct s_variables	*next;
}						t_variables;

typedef struct			s_variables_list
{
	t_variables			*head;
	t_variables			*tail;
	int					node_count;
}						t_variables_list;

t_variables_list		*g_env;

#endif
