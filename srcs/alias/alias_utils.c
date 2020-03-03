/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 13:11:58 by cjamal            #+#    #+#             */
/*   Updated: 2020/03/03 01:28:40 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** Function responsible for inserting a new alias to list
*/

void			alias_insert(t_alias_list *list, char *args)
{
	size_t			len;
	t_alias			*current;
	char			**alias;
	char			*tmp;

	current = NULL;
	if (list && list->head)
		current = list->head;
	alias = split_eq(args);
	len = ft_strlen(alias[0]);
	while (current)
	{
		if (len == current->alias_len && strcmp(alias[0], current->alias) == 0)
		{
			tmp = current->value;
			current->value = alias[1];
			ft_strdel(&tmp);
			free(alias[0]);
			free(alias);
			return ;
		}
		current = current->next;
	}
	alias_push(list, alias[0], alias[1]);
	free(alias);
}

char			*get_alias(char *key)
{
	t_alias_list	*list;
	char			*value;

	list = get_alias_list(NULL);
	value = get_alias_value(list, key);
	return (value);
}
