/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/13 23:23:56 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/28 16:30:01 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"

int		get_virtua_line_count(t_readline *readline)
{
	int			count;
	int			i;
	const int	*details = readline->line_props.details;
	const int	lc = readline->line_props.linecount - 1;

	i = 0;
	count = !!((details[i] + readline->o_cursor.x + (i == lc)) % readline->col);
	count += ((details[i] + readline->o_cursor.x + (i == lc)) / readline->col);
	i++;
	while (i < readline->line_props.linecount)
	{
		count += !!((details[i] + (i == lc)) % readline->col);
		count += ((details[i] + (i == lc)) / readline->col);
		i++;
	}
	return (count);
}

void	update_o_v_cursor(t_readline *env)
{
	const t_point o_c = env->o_cursor;
	const t_point o_v = env->ov_cursor;

	get_cursor_position(env);
	env->o_cursor = (t_point){o_c.x, o_c.y + (env->o_cursor.y - o_v.y)};
	set_virtual_origin(env);
}

void	update_o_cursor(t_readline *env)
{
	int	diff;

	diff = (get_virtua_line_count(env) + env->o_cursor.y) - env->row;
	if (diff > 0)
	{
		env->o_cursor.y -= diff;
		set_virtual_origin(env);
	}
}

void	get_cursor_position(t_readline *readline)
{
	char	buf;
	int		column;
	int		row;
	char	begin;

	begin = 0;
	column = 0;
	row = 0;
	tputs("\e[6n", 1, ft_putchar);
	while (read(0, &buf, 1) > 0)
	{
		if (begin == 3 && buf == 'R')
			break ;
		else if (begin == 2 && buf == ';')
			begin = 3;
		else if (begin == 0 && buf == 91)
			begin = 2;
		else if (begin == 2)
			row = (row * 10) + (buf - '0');
		else if (begin == 3)
			column = (column * 10) + (buf - '0');
	}
	readline->o_cursor = (t_point){column - 1, row - 1};
	readline->ov_cursor = readline->o_cursor;
}

char	*remove_unprintable_chars(char *str)
{
	char	*ret;
	int		i;

	ret = str;
	i = 0;
	if (str)
	{
		while (*str)
		{
			if (ft_isprint(*str) || *str == '\n')
			{
				ret[i++] = *str;
			}
			str++;
		}
		ret[i] = 0;
	}
	return (ret);
}

int		put_char(int c)
{
	ft_putchar(c);
	return (0);
}
