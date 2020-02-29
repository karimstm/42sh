/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/03 16:16:54 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/29 13:11:38 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void			alias_push(t_alias_list *list, char *alias, char *value)
{
	t_alias	*new_alias;

	new_alias = (t_alias *)malloc(sizeof(t_alias));
	new_alias->alias = alias;
	new_alias->value = value;
	new_alias->alias_len = ft_strlen(alias);
	new_alias->next = NULL;
	if (list->node_count == 0 || list->head == NULL)
		list->head = new_alias;
	else
		list->tail->next = new_alias;
	list->tail = new_alias;
	list->node_count++;
}

int				is_alias_pattern(char *str)
{
	char *start;

	start = str;
	while (*start)
	{
		if (*start == '=')
			return (start - str);
		start++;
	}
	return (0);
}

char			**split_eq(char *str)
{
	char		**list;
	const int	len = is_alias_pattern(str);

	list = (char **)malloc(sizeof(char *) * 3);
	list[0] = ft_strndup(str, len);
	list[1] = ft_strdup(&str[len + 1]);
	list[2] = NULL;
	return (list);
}

void			delete_alias(t_alias_list *list, char *to_find)
{
	t_alias			*current;
	t_alias			*next;
	t_alias			*prev;
	const size_t	len = ft_strlen(to_find);

	prev = NULL;
	current = (list && list->head) ? list->head : NULL;
	while (current)
	{
		if (len == current->alias_len && strcmp(to_find, current->alias) == 0)
		{
			next = current->next;
			if (current == list->head)
				list->head = next;
			if (current == list->tail)
				list->tail = prev;
			free(current->alias);
			free(current->value);
			free(current);
			list->node_count--;
			current = next;
		}
		prev = current;
		current = current->next;
	}
}

void			print_aliases(t_alias_list *list)
{
	t_alias *current;

	current = NULL;
	if (list && list->head)
		current = list->head;
	while (current)
	{
		printf("alias: %s='%s'\n", current->alias, current->value);
		current = current->next;
	}
}
