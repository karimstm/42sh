/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/03 16:18:40 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/29 13:56:34 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALIAS_H
# define ALIAS_H

# include <stdlib.h>
# include <string.h>
# include <stdio.h>

typedef struct			s_alias
{
	char				*alias;
	char				*value;
	size_t				alias_len;
	struct s_alias		*next;
}						t_alias;

typedef struct			s_alias_list
{
	t_alias				*head;
	t_alias				*tail;
	size_t				node_count;
}						t_alias_list;

/*
**	alias.c
*/
int						ft_alias(char **args);
void					init_alias(t_alias_list *list);
int						ft_unalias(char **args);
char					*get_alias_value(t_alias_list *list,
									char *alias);
t_alias_list			*get_alias_list(t_alias_list *aliases);
char					*get_alias(char *key);
t_alias					*alias_find(t_alias_list *list, char *alias);
char					**split_eq(char *str);
void					alias_push(t_alias_list *list,
							char *alias, char *value);
int						print_invalid_option(char c);
void					alias_insert(t_alias_list *list, char *args);
void					delete_alias(t_alias_list *list, char *to_find);
void					aliase_prune(t_alias_list *list);
void					print_aliases(t_alias_list *list);
int						is_alias_pattern(char *str);

#endif
