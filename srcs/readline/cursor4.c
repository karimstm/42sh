/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoulhafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 17:39:00 by zoulhafi          #+#    #+#             */
/*   Updated: 2019/04/22 00:29:19 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	update_index(t_line *line, char step)
{
	int		top;
	int		index;

	line->index += step;
	line->current_index += step;
	if (step == 1 && line->command[line->index] == '\n')
		line->current_index = -1;
	else if (step == -1 && line->command[line->index + 1] == '\n')
	{
		index = line->index;
		while (index >= 0 && line->command[index] != '\n')
			index--;
		top = -1;
		while (line->command[++index] != '\0' && line->command[index] != '\n')
			top++;
		line->current_index = top;
	}
}

int		decision_up_down(t_line *line)
{
	int		marge;

	marge = 0;
	if (line->index == line->current_index)
		marge = ft_strlen(GET_MSG(line->print_msg));
	if ((line->current_index + marge) % line->col == line->col - 1)
		return (1);
	else
		return (0);
}

int		get_current_rows(t_line *line)
{
	int	i;
	int	j;
	int	count_rows;

	count_rows = 0;
	i = line->index;
	if (line->index == line->current_index)
		j = line->current_index + 1 + ft_strlen(GET_MSG(line->print_msg));
	else
		j = line->current_index + 1;
	while (line->command[++i] != '\0')
	{
		if (line->command[i] == '\n')
		{
			count_rows++;
			j = 0;
		}
		else if (j % line->col == line->col - 1)
			count_rows++;
		j++;
	}
	return (count_rows);
}

int		decision_top_down_left(t_line *line, int current_rows)
{
	int		marge;
	int		top;
	int		index;
	int		height;

	marge = 0;
	index = line->index;
	while (index >= 0 && line->command[index] != '\n')
		index--;
	top = -1;
	while (line->command[++index] != '\0' && line->command[index] != '\n')
		top++;
	if (line->index == line->current_index)
		marge = ft_strlen(GET_MSG(line->print_msg));
	if ((top + marge) % line->col == line->col - 1)
	{
		height = tgetnum("li");
		if (get_current_row(height) + current_rows == height)
			return (1);
		else
			return (0);
	}
	else
		return (0);
}

void	set_new_current_index(t_line *line)
{
	int		index;

	index = line->index - 1;
	while (index >= 0 && line->command[index] != '\n')
		index--;
	if (index > 0)
		line->current_index = line->index - index;
	else
		line->current_index = line->index;
}
