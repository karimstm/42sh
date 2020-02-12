/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fc_l.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoulhafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 06:03:16 by zoulhafi          #+#    #+#             */
/*   Updated: 2020/02/11 06:03:18 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static t_cmd_history	*get_specific_history(int index)
{
	char			from_begin;
	t_cmd_history	*history;

	if (500 - index > 250)
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
	ft_printf("fc: history specification out of range\n");
	return (NULL);
}

static t_cmd_history	*get_specific_history_by_str(char *first)
{
	t_cmd_history	*history;

	history = get_cmd_history_head();
	while (history)
	{
		if (ft_strncmp(history->line, first, ft_strlen(first)) == 0)
			return (history);
		history = history->next;
	}
	ft_printf("fc: history specification out of range\n");
	return (NULL);
}

static t_cmd_history	*get_first(char *first)
{
	char			first_is_number;
	int				count;
	int				max;
	t_cmd_history	*history;

	first_is_number = 0;
	if (first != NULL && ft_strlen(first) == ft_strlen(ft_itoa(ft_atoi(first))))
		first_is_number = 1;
	if (first != NULL && first_is_number && first[0] != '-')
		return (get_specific_history(ft_atoi(first)));
	else if (first != NULL && !first_is_number)
		return (get_specific_history_by_str(first));
	history = get_cmd_history_head();
	max = 16;
	if (first != NULL && first_is_number && first[0] == '-')
		max = ft_atoi(first) * -1;
	count = 1;
	while (history && count < max)
	{
		history = history->next;
		count++;
	}
	return (history == NULL ? history_begining : history);
}

static t_cmd_history	*get_last(char *last)
{
	char			last_is_number;
	int				count;
	int				max;
	t_cmd_history	*history;

	last_is_number = 0;
	if (last != NULL && ft_strlen(last) == ft_strlen(ft_itoa(ft_atoi(last))))
		last_is_number = 1;
	if (last != NULL && last_is_number && last[0] != '-')
		return (get_specific_history(ft_atoi(last)));
	else if (last != NULL && !last_is_number)
		return (get_specific_history_by_str(last));
	history = get_cmd_history_head();
	if (last != NULL && last_is_number && last[0] == '-')
	{
		max = ft_atoi(last) * -1;
		count = 1;
		while (history && count < max)
		{
			history = history->next;
			count++;
		}
	}
	return (history == NULL ? history_begining : history);
}

void					fc_l(int flags, char *first, char *last)
{
	t_cmd_history	*first_h;
	t_cmd_history	*last_h;
	t_cmd_history	*history;

	first_h = get_first(first);
	last_h = get_last(last);
	if (first_h && last_h && last_h->index - first_h->index < 0 &&
			(flags |= R_FLAG) >= 0)
		ft_swap_pt((void *)&first_h, (void *)&last_h);
	history = first_h;
	if ((flags & R_FLAG) > 0)
		history = last_h;
	while (history)
	{
		if ((flags & N_FLAG) > 0)
			ft_printf("\t%s\n", history->line);
		else
			ft_printf("%d\t%s\n", history->index, history->line);
		if (((flags & R_FLAG) > 0 && history == first_h) ||
			((flags & R_FLAG) <= 0 && history == last_h))
			break ;
		history = (flags & R_FLAG) > 0 ? history->next : history->prev;
	}
}
