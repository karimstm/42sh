/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoulhafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 06:03:16 by zoulhafi          #+#    #+#             */
/*   Updated: 2020/02/11 06:03:18 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_cmd_history	*get_specific_history(int index, char is_l)
{
	int				total;
	char			from_begin;
	t_cmd_history	*history;

	total = get_cmd_history_head()->index;
	if (total - index > total / 2)
		from_begin = 1;
	else
		from_begin = 0;
	if (from_begin)
		history = history_begining;
	else
		history = get_cmd_history_head();
	while (history)
	{
		if (history->index == index)
			return (history);
		if (from_begin)
			history = history->prev;
		else
			history = history->next;
	}
	return (is_l ? get_cmd_history_head() : NULL);
}

t_cmd_history	*get_history_by_reverse(int max)
{
	int				count;
	t_cmd_history	*history;

	history = get_cmd_history_head();
	count = 1;
	while (history && count < max)
	{
		history = history->next;
		count++;
	}
	return (history);
}

t_cmd_history	*get_specific_history_by_str(char *first, char *error_msg)
{
	t_cmd_history	*history;

	history = get_cmd_history_head();
	while (history)
	{
		if (ft_strncmp(history->line, first, ft_strlen(first)) == 0)
			return (history);
		history = history->next;
	}
	ft_printf(error_msg);
	return (NULL);
}
