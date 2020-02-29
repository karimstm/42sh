/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_alias.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 13:07:38 by cjamal            #+#    #+#             */
/*   Updated: 2020/02/29 13:19:45 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_alias_list	*get_alias_list(t_alias_list *aliases)
{
	static t_alias_list	*list = NULL;

	if (aliases != NULL)
		list = aliases;
	return (list);
}

void			init_alias(t_alias_list *list)
{
	list->head = NULL;
	list->tail = NULL;
	list->node_count = 0;
}

t_alias			*alias_find(t_alias_list *list, char *alias)
{
	t_alias			*current;
	const size_t	len = ft_strlen(alias);

	current = NULL;
	if (list && list->head)
		current = list->head;
	while (current)
	{
		if (len == current->alias_len && strcmp(alias, current->alias) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void			aliase_prune(t_alias_list *list)
{
	t_alias *current;
	t_alias	*tmp;

	current = NULL;
	if (list && list->head)
		current = list->head;
	while (current)
	{
		tmp = current->next;
		free(current->alias);
		free(current->value);
		free(current);
		current = NULL;
		current = tmp;
	}
	init_alias(list);
	free(list);
	list = NULL;
}

char			*get_alias_value(t_alias_list *list,
								char *alias)
{
	t_alias_list	*tmp_list;

	DECLARE(char, _(*value, NULL));
	DECLARE(size_t, _(len, ft_strlen(alias)));
	DECLARE(t_alias, _(*cur, (list && list->head) ? list->head : NULL), *tmp);
	init_alias((tmp_list = (t_alias_list *)malloc(sizeof(t_alias_list))));
	while (cur)
	{
		if (len == cur->alias_len && strcmp(alias, cur->alias) == 0)
		{
			if ((tmp = alias_find(tmp_list, alias)))
			{
				value = tmp->value;
				break ;
			}
			value = cur->value;
			alias_push(tmp_list, ft_strdup(alias), ft_strdup(value));
			alias = value;
			len = ft_strlen(alias);
			cur = list->head;
		}
		cur = cur->next;
	}
	aliase_prune(tmp_list);
	return (value);
}
