/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auto_compilation.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szakaria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/01 04:22:35 by szakaria          #+#    #+#             */
/*   Updated: 2020/03/01 04:33:15 by szakaria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTO_COMPILATION_H
# define AUTO_COMPILATION_H
# include "../ft_readline.h"
# include <stdio.h>
# include <dirent.h>
# include <stdlib.h>
# include <termios.h>

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

t_variables_list		*env2;

#endif
