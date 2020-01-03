/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/03 16:18:40 by amoutik           #+#    #+#             */
/*   Updated: 2020/01/03 16:18:41 by amoutik          ###   ########.fr       */
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
void					ft_alias(char **args, t_alias_list *list);
void					init_alias(t_alias_list *list);
void					ft_unalias(char **args, t_alias_list *list);
char					*get_alias_value(t_alias_list *list,
									char *alias, char *start_alias);

#endif
