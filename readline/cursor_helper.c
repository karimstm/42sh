/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/06 14:44:38 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/10 23:52:54 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"

void	set_virtual_origin(t_readline *readline)
{
	int		i;
	t_point	origin;

	origin = readline->o_cursor;
	i = readline->line_props.index;
	while (i > 0)
	{
		i--;
		origin.y += readline->line_props.details[i] / readline->col;
		origin.y += (readline->line_props.details[i] % readline->col) > 0;
		origin.x = 0;
	}
	readline->ov_cursor = origin;
}

void	set_idnex_from_cursor(t_readline *readline)
{
	int		index;
	int		i;

	i = readline->line_props.index;
	index = readline->cursor;
	if (!BETWEEN(index, 0, readline->line_props.details[i]))
		index = 0;
	while (i)
	{
		i--;
		index += readline->line_props.details[i];
	}
	readline->line_index = index;
}

void	set_cursor_from_index(t_readline *readline)
{
	int			index;
	int			i;
	const int	*details = readline->line_props.details;

	i = 0;
	index = 0;
	if (readline->cmd->tmp_len == readline->line_index)
	{
		readline->line_props.index = readline->line_props.linecount - 1;
		readline->cursor = details[readline->line_props.index];
		set_virtual_origin(readline);
		return ;
	}
	while (1)
	{
		if (i >= readline->line_props.linecount)
			break ;
		if (BETWEEN(readline->line_index, index, index + details[i] - 1))
			break ;
		index += readline->line_props.details[i];
		i++;
	}
	readline->line_props.index = i % readline->line_props.linecount;
	readline->cursor = readline->line_index - index;
	set_virtual_origin(readline);
}

void	to_start_or_end(t_readline *readline, int button)
{
	const int	l = readline->line_props.details[readline->line_props.index];
	const int	count = readline->line_props.linecount;
	int			cursor;

	cursor = 0;
	if (button == BUTTON_HOME)
	{
		if (readline->cursor == 0 && readline->line_props.index > 0)
		{
			readline->line_props.index--;
			set_virtual_origin(readline);
		}
	}
	else
	{
		if (readline->cursor == l - 1 && readline->line_props.index < count - 1)
		{
			readline->line_props.index++;
			set_virtual_origin(readline);
		}
		cursor = readline->line_props.details[readline->line_props.index];
		cursor -= readline->line_props.index != count - 1;
	}
	cur_goto(readline, cursor);
}

void	cur_goto(t_readline *readline, int cursor)
{
	t_point	origin;

	origin = readline->ov_cursor;
	origin.y += (origin.x + cursor) / readline->col;
	origin.x = (origin.x + cursor) % readline->col;
	readline->cursor = cursor;
	set_idnex_from_cursor(readline);
	tputs(tgoto(tgetstr("cm", 0), origin.x, origin.y), 0, put_char);
}
